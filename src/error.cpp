
#include "cmsis_compiler.h"
#include "rtx_os.h"
#include "misc.h"

/// OS Error Callback function
extern "C" uint32_t osRtxErrorNotify (uint32_t code, void *object_id);
uint32_t osRtxErrorNotify (uint32_t code, void *object_id)
{
    auto const &id = reinterpret_cast<uint32_t>(object_id);
    switch (code)
    {
        case osRtxErrorStackOverflow:
        {
            // Stack overflow detected for thread (thread_id=object_id)
            const char *name = osThreadGetName(object_id);
            printerr("Stack overflow detected for thread. ID = %#010x. Name = '%s'.\n", id, (name != nullptr) ? name : "undefined");
        }
        break;
        case osRtxErrorISRQueueOverflow:
        {
            // ISR Queue overflow detected when inserting object (object_id)
            printerr("ISR Queue overflow detected when inserting object. ID = %#010x.\n", id);
        }
        break;
        case osRtxErrorTimerQueueOverflow:
        {
            // User Timer Callback Queue overflow detected for timer (timer_id=object_id)
            const char *name = osTimerGetName(object_id);
            printerr("User Timer Callback Queue overflow detected for timer. ID = %#010x. Name = '%s'.\n", id, (name != nullptr) ? name : "undefined");
        }
        break;
        case osRtxErrorClibSpace:
        {
            // Standard C/C++ library libspace not available: increase OS_THREAD_LIBSPACE_NUM
            printerr("Standard C/C++ library libspace not available: increase OS_THREAD_LIBSPACE_NUM. ID = %#010x.\n", id);
        }
        break;
        case osRtxErrorClibMutex:
        {
            // Standard C/C++ library mutex initialization failed
            printerr("Standard C/C++ library mutex initialization failed. ID = %#010x.\n", id);
        }
        break;
        default:
        {
            // Reserved
            printerr("Unknown error.\n");
        }
    }
    for(;;);
}

/// System hard fault
extern "C" void HardFault_Handler(void);
__NO_RETURN void HardFault_Handler(void)
{
    printerr("Hard fault.\n");
    for(;;);
}
