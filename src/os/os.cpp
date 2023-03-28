#include "rtx_os.h"
#include "RTX_Config.h"

#include "os.h"

namespace os
{

template <typename T, typename OT = sts_t>
static OT chck(const T _sts)
{
    return static_cast<OT>(_sts);
}

// Timeout value.
constexpr uint32_t forever = 0xFFFFFFFFU; ///< Wait forever timeout value.

/// Wait for Timeout (Time Delay).
/// \param[in]     ticks         \ref CMSIS_RTOS_TimeOutValue "time ticks" value
/// \return status code that indicates the execution status of the function.
sts_t delay(const uint32_t _ticks)
{
    return chck(osDelay(_ticks));
}

/// Wait until specified time.
/// \param[in]     ticks         absolute time in ticks
/// \return status code that indicates the execution status of the function.
sts_t delay_until(const uint32_t _ticks)
{
    return chck(osDelayUntil(_ticks));
}
    
namespace kernel
{
    
///  Get RTOS Kernel Information.
// osStatus_t osKernelGetInfo (osVersion_t *version, char *id_buf, uint32_t id_size);
constexpr vers_t v_api  = osRtxVersionAPI;
constexpr vers_t v_krnl = osRtxVersionKernel;
constexpr char id[] = osRtxKernelId;

/// Initialize the RTOS Kernel.
/// \return status code that indicates the execution status of the function.
sts_t initialize(void)
{
    return chck(osKernelInitialize());
}

/// Get the current RTOS Kernel state.
/// \return current RTOS Kernel state.
ksts_t get_state(void)
{
    return chck<osKernelState_t, ksts_t>(osKernelGetState());
}

/// Start the RTOS Kernel scheduler.
/// \return status code that indicates the execution status of the function.
sts_t start(void)
{
    return chck(osKernelStart());
}

/// Lock the RTOS Kernel scheduler.
/// \return previous lock state (1 - locked, 0 - not locked, error code if negative).
sts_t lock(void)
{
    return chck(osKernelLock());
}

/// Unlock the RTOS Kernel scheduler.
/// \return previous lock state (1 - locked, 0 - not locked, error code if negative).
sts_t unlock(void)
{
    return chck(osKernelUnlock());
}

/// Restore the RTOS Kernel scheduler lock state.
/// \param[in]     lock          lock state obtained by \ref osKernelLock or \ref osKernelUnlock.
/// \return new lock state (1 - locked, 0 - not locked, error code if negative).
sts_t restore_lock(const sts_t _lock)
{
    return chck(osKernelRestoreLock(static_cast<int32_t>(_lock)));
}

/// Suspend the RTOS Kernel scheduler.
/// \return time in ticks, for how long the system can sleep or power-down.
uint32_t suspend(void)
{
    return osKernelSuspend();
}

/// Resume the RTOS Kernel scheduler.
/// \param[in]     sleep_ticks   time in ticks for how long the system was in sleep or power-down mode.
void resume(const uint32_t _sleep_ticks)
{
    osKernelResume(_sleep_ticks);
}

/// Get the RTOS kernel tick count.
/// \return RTOS kernel current tick count.
uint32_t get_tick_count(void)
{
    return osKernelGetTickCount();
}

/// Get the RTOS kernel tick frequency.
/// \return frequency of the kernel tick in hertz, i.e. kernel ticks per second.
uint32_t get_tick_freq(void)
{
    return osKernelGetTickFreq();
}

/// Get the RTOS kernel system timer count.
/// \return RTOS kernel current system timer count as 32-bit value.
uint32_t get_sys_timer_count(void)
{
    return osKernelGetSysTimerCount();
}

/// Get the RTOS kernel system timer frequency.
/// \return frequency of the system timer in hertz, i.e. timer ticks per second.
uint32_t get_sys_timer_freq(void)
{
    return osKernelGetSysTimerFreq();
}

} // namespace kernel

} // namespace os
