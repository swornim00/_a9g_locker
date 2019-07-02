
#include "stdbool.h"
#include "stdint.h"

#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"


#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "Main Task"

#define LOCKER_TASK_STACK_SIZE    (2048 * 2)
#define LOCKER_TASK_PRIORITY      1
#define LOCKER_TASK_NAME          "Get SMS Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE lockerTaskHandle = NULL;


void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_NO_SIMCARD:
            Trace("[!] No Sim Card");
        case API_EVENT_ID_SYSTEM_READY:
            Trace("[*] System is Ready!");

        case API_EVENT_ID_SYSTEM_READY:
            Trace(1,"[*] System initialize complete");
            break;
        
        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
            Trace(2,"[*] Network register success");
            break;
            
        case API_EVENT_ID_SMS_RECEIVED:
            Trace(2,"received message");

            SMS_Encode_Type_t encodeType = pEvent->param1;
            uint32_t contentLength = pEvent->param2;
            uint8_t* header = pEvent->pParam1;
            uint8_t* content = pEvent->pParam2;

            Trace(2,"[*] Message header:%s",header);
            Trace(2,"[*] message content length:%d",contentLength);

            if(encodeType == SMS_ENCODE_TYPE_ASCII)
            {
                Trace(2,"[*] Message content:%s",content);
                UART_Write(UART1,content,contentLength);
            }else{
                Trace(2,"[!] The Message is not in ASCII")
            }
            break;
        case API_EVENT_ID_SMS_ERROR:
            Trace(10,"[!] SMS error occured! cause:%d",pEvent->param1);
        default:
            break;
    }
}

void LockerTask(void *pData)
{
    // We will Start Our Code Here!
}

void MainTask(void *pData)
{
    API_Event_t* event=NULL;

    lockerTaskHandle = OS_CreateTask(LockerTask,
        NULL, NULL, LOCKER_TASK_STACK_SIZE, LOCKER_TASK_PRIORITY, 0, 0, LOCKER_TASK_NAME);

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

void locker_Main(void)
{
    mainTaskHandle = OS_CreateTask(MainTask,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}