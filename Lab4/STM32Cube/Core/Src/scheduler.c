
#include "main.h"
#include "scheduler.h"

#define NO_TASK (-1)

typedef struct {
    void (*pTask)(void);
    uint32_t delta;
    uint32_t period;
    uint32_t taskID;
    int16_t next;
    uint8_t active;
} sTask;

static sTask SCH_tasks[SCH_MAX_TASKS];

static volatile uint32_t system_ticks = 0;
static volatile uint32_t pending_ticks = 0;

static uint32_t next_task_id = 1;
static int16_t head = NO_TASK;


/* Critical-section helpers.
   Preserve the previous interrupt state. */
static uint32_t SCH_Enter_Critical(void)
{
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    return primask;
}

static void SCH_Exit_Critical(uint32_t primask)
{
    __set_PRIMASK(primask);
}


/* Insert an existing task into the delta list.
   DELAY is relative to the current scheduler time. */
static void SCH_Insert_Task(int16_t taskIndex, uint32_t DELAY)
{
    int16_t *link = &head;
    uint32_t remaining = DELAY;

    while (*link != NO_TASK &&
           remaining >= SCH_tasks[*link].delta) {

        remaining -= SCH_tasks[*link].delta;
        link = &SCH_tasks[*link].next;
    }

    SCH_tasks[taskIndex].delta = remaining;
    SCH_tasks[taskIndex].next = *link;

    if (*link != NO_TASK) {
        SCH_tasks[*link].delta -= remaining;
    }

    *link = taskIndex;
}


/* Apply elapsed ticks accumulated while the head task
   was already due.

   Called only from the main context with interrupts disabled. */
static void SCH_Apply_Pending_Ticks(void)
{
    uint32_t elapsed = pending_ticks;
    pending_ticks = 0;

    if (head == NO_TASK || elapsed == 0) {
        return;
    }

    /* The head is already due.
       Apply elapsed time to the tasks behind it. */
    int16_t current = SCH_tasks[head].next;

    while (current != NO_TASK && elapsed > 0) {

        if (elapsed >= SCH_tasks[current].delta) {

            elapsed -= SCH_tasks[current].delta;
            SCH_tasks[current].delta = 0;

            current = SCH_tasks[current].next;
        }
        else {
            SCH_tasks[current].delta -= elapsed;
            elapsed = 0;
        }
    }
}


void SCH_Init(void)
{
    uint32_t primask = SCH_Enter_Critical();

    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_tasks[i].pTask = 0;
        SCH_tasks[i].delta = 0;
        SCH_tasks[i].period = 0;
        SCH_tasks[i].taskID = 0;
        SCH_tasks[i].next = NO_TASK;
        SCH_tasks[i].active = 0;
    }

    head = NO_TASK;
    system_ticks = 0;
    pending_ticks = 0;
    next_task_id = 1;

    SCH_Exit_Critical(primask);
}


/* Called every 10 ms by the timer interrupt.
   O(1): no task-list traversal. */
void SCH_Update(void)
{
    system_ticks++;

    if (head != NO_TASK) {

        if (SCH_tasks[head].delta > 0) {
            SCH_tasks[head].delta--;
        }
        else {
            /* Head is already due.
               Accumulate elapsed time for later processing. */
            pending_ticks++;
        }
    }
}


uint32_t get_time(void)
{
    uint32_t primask = SCH_Enter_Critical();

    uint32_t time = system_ticks * SCH_TICK_MS;

    SCH_Exit_Critical(primask);

    return time;
}


uint32_t SCH_Add_Task(void (*pFunction)(void),
                      uint32_t DELAY,
                      uint32_t PERIOD)
{
    if (pFunction == 0 || DELAY >= 0x80000000U ||
        PERIOD >= 0x80000000U) {
        return 0;
    }

    uint32_t primask = SCH_Enter_Critical();

    SCH_Apply_Pending_Ticks();

    int16_t freeIndex = NO_TASK;

    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        if (!SCH_tasks[i].active) {
            freeIndex = i;
            break;
        }
    }

    if (freeIndex == NO_TASK) {
        SCH_Exit_Critical(primask);
        return 0;
    }

    SCH_tasks[freeIndex].pTask = pFunction;
    SCH_tasks[freeIndex].period = PERIOD;
    SCH_tasks[freeIndex].taskID = next_task_id++;

    if (next_task_id == 0) {
        next_task_id = 1;
    }

    SCH_tasks[freeIndex].active = 1;
    SCH_tasks[freeIndex].next = NO_TASK;

    SCH_Insert_Task(freeIndex, DELAY);

    uint32_t id = SCH_tasks[freeIndex].taskID;

    SCH_Exit_Critical(primask);

    return id;
}


uint8_t SCH_Delete_Task(uint32_t taskID)
{
    uint32_t primask = SCH_Enter_Critical();

    SCH_Apply_Pending_Ticks();

    int16_t previous = NO_TASK;
    int16_t current = head;

    while (current != NO_TASK) {

        if (SCH_tasks[current].taskID == taskID) {

            int16_t next = SCH_tasks[current].next;

            /* Transfer this task's remaining delta
               to its successor. */
            if (next != NO_TASK) {
                SCH_tasks[next].delta +=
                    SCH_tasks[current].delta;
            }

            if (previous == NO_TASK) {
                head = next;
            }
            else {
                SCH_tasks[previous].next = next;
            }

            SCH_tasks[current].pTask = 0;
            SCH_tasks[current].active = 0;
            SCH_tasks[current].next = NO_TASK;

            SCH_Exit_Critical(primask);
            return 1;
        }

        previous = current;
        current = SCH_tasks[current].next;
    }

    SCH_Exit_Critical(primask);
    return 0;
}


void SCH_Dispatch_Tasks(void)
{
    while (1) {

        uint32_t primask = SCH_Enter_Critical();

        SCH_Apply_Pending_Ticks();

        /* No task is currently due. */
        if (head == NO_TASK ||
            SCH_tasks[head].delta != 0) {

            SCH_Exit_Critical(primask);
            break;
        }

        /* Remove the due task from the head. */
        int16_t current = head;
        head = SCH_tasks[current].next;

        void (*task)(void) = SCH_tasks[current].pTask;

        uint32_t period = SCH_tasks[current].period;

        if (period == 0) {

            /* One-shot task. */
            SCH_tasks[current].pTask = 0;
            SCH_tasks[current].active = 0;
            SCH_tasks[current].next = NO_TASK;
        }
        else {

            /* Periodic task: schedule its next execution
               relative to the current scheduler time. */
            SCH_tasks[current].next = NO_TASK;
            SCH_Insert_Task(current, period);
        }

        SCH_Exit_Critical(primask);

        /* Execute outside the critical section and ISR. */
        if (task != 0) {
            task();
        }
    }
}
