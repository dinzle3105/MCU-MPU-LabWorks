#include "scheduler.h"

typedef struct {
    void (*pTask)(void);
    uint32_t due_tick;
    uint32_t period;
    uint32_t taskID;
    uint8_t active;
} sTask;

static sTask SCH_tasks[SCH_MAX_TASKS];
static volatile uint32_t system_ticks = 0;
static uint32_t next_task_id = 1;

void SCH_Init(void)
{
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_tasks[i].pTask = 0;
        SCH_tasks[i].active = 0;
    }

    system_ticks = 0;
    next_task_id = 1;
}

void SCH_Update(void)
{
    // Called every 10 ms by TIM2 interrupt.
    // No task-array scanning here.
    system_ticks++;
}

uint32_t get_time(void)
{
    return system_ticks * SCH_TICK_MS;
}

uint32_t SCH_Add_Task(void (*pFunction)(void),
                      uint32_t DELAY,
                      uint32_t PERIOD)
{
    if (pFunction == 0) return 0;

    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        if (!SCH_tasks[i].active) {
            SCH_tasks[i].pTask = pFunction;
            SCH_tasks[i].due_tick = system_ticks + DELAY;
            SCH_tasks[i].period = PERIOD;
            SCH_tasks[i].taskID = next_task_id++;
            SCH_tasks[i].active = 1;

            return SCH_tasks[i].taskID;
        }
    }

    return 0; // Task list full
}

uint8_t SCH_Delete_Task(uint32_t taskID)
{
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        if (SCH_tasks[i].active &&
            SCH_tasks[i].taskID == taskID) {

            SCH_tasks[i].active = 0;
            SCH_tasks[i].pTask = 0;
            return 1;
        }
    }

    return 0; // Task not found
}

void SCH_Dispatch_Tasks(void)
{
    while (1) {
        int selected = -1;
        uint32_t now = system_ticks;

        // Find the earliest task that is due.
        for (int i = 0; i < SCH_MAX_TASKS; i++) {
            if (!SCH_tasks[i].active) continue;

            if ((int32_t)(now - SCH_tasks[i].due_tick) >= 0) {
                if (selected == -1 ||
                    (int32_t)(SCH_tasks[i].due_tick -
                              SCH_tasks[selected].due_tick) < 0) {
                    selected = i;
                }
            }
        }

        if (selected == -1) break;

        void (*task)(void) = SCH_tasks[selected].pTask;

        if (SCH_tasks[selected].period == 0) {
            // One-shot task: remove before executing.
            SCH_tasks[selected].active = 0;
            SCH_tasks[selected].pTask = 0;
        } else {
            // Periodic task: schedule its next execution.
            SCH_tasks[selected].due_tick += SCH_tasks[selected].period;
        }

        // Execute outside the timer interrupt.
        task();
    }
}
