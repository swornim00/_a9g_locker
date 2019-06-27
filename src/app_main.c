
#include "stdbool.h"
#include "stdint.h"

#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"


#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Main Task"

#define GETSMS_TASK_STACK_SIZE    (2048 * 2)
#define GETMS_TASK_PRIORITY      1
#define GETSMS_TASK_NAME          "Second Test Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE getsmsTaskHandle = NULL;


void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        default:
            break;
    }
}

void GetSMSTask(void *pData)
{
    // We will Start Our Code Here!
}

void MainTask(void *pData)
{
    API_Event_t* event=NULL;

    getsmsTaskHandle = OS_CreateTask(GetSMSTask,
        NULL, NULL, SECOND_TASK_STACK_SIZE, SECOND_TASK_PRIORITY, 0, 0, SECOND_TASK_NAME);

    while(1)
    {
        if(OS_WaitEvent(mainTaskHandle, (void**)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}

void app_Main(void)
{
    mainTaskHandle = OS_CreateTask(MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}