
namespace os
{

/// Priority values.
enum class priority : int32_t
{
    na             =  0,         ///< No priority (not initialized).
    idle           =  1,         ///< Reserved for Idle thread.
    low            =  8,         ///< Priority: low
    low1           =  8 + 1,     ///< Priority: low + 1
    low2           =  8 + 2,     ///< Priority: low + 2
    low3           =  8 + 3,     ///< Priority: low + 3
    low4           =  8 + 4,     ///< Priority: low + 4
    low5           =  8 + 5,     ///< Priority: low + 5
    low6           =  8 + 6,     ///< Priority: low + 6
    low7           =  8 + 7,     ///< Priority: low + 7
    below_normal   = 16,         ///< Priority: below normal
    below_normal1  = 16 + 1,     ///< Priority: below normal + 1
    below_normal2  = 16 + 2,     ///< Priority: below normal + 2
    below_normal3  = 16 + 3,     ///< Priority: below normal + 3
    below_normal4  = 16 + 4,     ///< Priority: below normal + 4
    below_normal5  = 16 + 5,     ///< Priority: below normal + 5
    below_normal6  = 16 + 6,     ///< Priority: below normal + 6
    below_normal7  = 16 + 7,     ///< Priority: below normal + 7
    normal         = 24,         ///< Priority: normal
    normal1        = 24 + 1,     ///< Priority: normal + 1
    normal2        = 24 + 2,     ///< Priority: normal + 2
    normal3        = 24 + 3,     ///< Priority: normal + 3
    normal4        = 24 + 4,     ///< Priority: normal + 4
    normal5        = 24 + 5,     ///< Priority: normal + 5
    normal6        = 24 + 6,     ///< Priority: normal + 6
    normal7        = 24 + 7,     ///< Priority: normal + 7
    above_normal   = 32,         ///< Priority: above normal
    above_normal1  = 32 + 1,     ///< Priority: above normal + 1
    above_normal2  = 32 + 2,     ///< Priority: above normal + 2
    above_normal3  = 32 + 3,     ///< Priority: above normal + 3
    above_normal4  = 32 + 4,     ///< Priority: above normal + 4
    above_normal5  = 32 + 5,     ///< Priority: above normal + 5
    above_normal6  = 32 + 6,     ///< Priority: above normal + 6
    above_normal7  = 32 + 7,     ///< Priority: above normal + 7
    high           = 40,         ///< Priority: high
    high1          = 40 + 1,     ///< Priority: high + 1
    high2          = 40 + 2,     ///< Priority: high + 2
    high3          = 40 + 3,     ///< Priority: high + 3
    high4          = 40 + 4,     ///< Priority: high + 4
    high5          = 40 + 5,     ///< Priority: high + 5
    high6          = 40 + 6,     ///< Priority: high + 6
    high7          = 40 + 7,     ///< Priority: high + 7
    realtime       = 48,         ///< Priority: realtime
    realtime1      = 48 + 1,     ///< Priority: realtime + 1
    realtime2      = 48 + 2,     ///< Priority: realtime + 2
    realtime3      = 48 + 3,     ///< Priority: realtime + 3
    realtime4      = 48 + 4,     ///< Priority: realtime + 4
    realtime5      = 48 + 5,     ///< Priority: realtime + 5
    realtime6      = 48 + 6,     ///< Priority: realtime + 6
    realtime7      = 48 + 7,     ///< Priority: realtime + 7
    ISR            = 56,         ///< Reserved for ISR deferred thread.
    err            = -1,         ///< System cannot determine priority or illegal priority.
    reserved       = 0x7FFFFFFF  ///< Prevents enum down-size compiler optimization.
};

/// Status code values returned by CMSIS-RTOS functions.
enum class sts_t : int32_t
{
    locked                  =  1,         ///< Lock state - locked. See @ref kernel::lock, @ref kernel::unlock.
    not_locked              =  0,         ///< Lock state - not locked. See @ref kernel::lock, @ref kernel::unlock.
    OK                      =  0,         ///< Operation completed successfully.
    err                     = -1,         ///< Unspecified RTOS error: run-time error but no other error message fits.
    err_timeout             = -2,         ///< Operation not completed within the timeout period.
    err_resource            = -3,         ///< Resource not available.
    err_parameter           = -4,         ///< Parameter error.
    err_nomem               = -5,         ///< System is out of memory: it was impossible to allocate or reserve memory for the operation.
    err_ISR                 = -6,         ///< Not allowed in ISR context: the function cannot be called from interrupt service routines.
    reserved                = 0x7FFFFFFF  ///< Prevents enum down-size compiler optimization.
};

/// Timeout value.
extern const uint32_t forever; ///< Wait forever timeout value.

/// Wait for Timeout (Time Delay).
/// \param[in]     ticks         \ref CMSIS_RTOS_TimeOutValue "time ticks" value
/// \return status code that indicates the execution status of the function.
sts_t delay(const uint32_t _ticks);

/// Wait until specified time.
/// \param[in]     ticks         absolute time in ticks
/// \return status code that indicates the execution status of the function.
sts_t delay_until(const uint32_t _ticks);

///  ==== Kernel Management Functions ====
namespace kernel
{

struct vers_t;
struct vers_t
{
    const uint32_t raw;

    uint16_t rev()
    {
        return static_cast<uint16_t>(raw / 1000);
    }
    uint16_t minor()
    {
        return static_cast<uint16_t>((raw % 10000000) / 1000);
    }
    uint8_t major()
    {
        return static_cast<uint8_t>(raw / 10000000);
    }

    constexpr vers_t(const uint32_t &_raw): raw(_raw) {}
};

/// Kernel state.
enum class ksts_t : int32_t
{
    inactive        =  0,         ///< Inactive.
    ready           =  1,         ///< Ready.
    running         =  2,         ///< Running.
    locked          =  3,         ///< Locked.
    suspended       =  4,         ///< Suspended.
    err             = -1,         ///< Error.
    reserved        = 0x7FFFFFFF  ///< Prevents enum down-size compiler optimization.
};

/// Initialize the RTOS Kernel.
/// \return status code that indicates the execution status of the function.
extern sts_t initialize(void);

///  Get RTOS Kernel Information.
// osStatus_t osKernelGetInfo (osVersion_t *version, char *id_buf, uint32_t id_size);
extern const vers_t v_api;
extern const vers_t v_krnl;
extern const char id[];


/// Get the current RTOS Kernel state.
/// \return current RTOS Kernel state.
inline ksts_t get_state(void);

/// Start the RTOS Kernel scheduler.
/// \return status code that indicates the execution status of the function.
sts_t start(void);

/// Lock the RTOS Kernel scheduler.
/// \return previous lock state (1 - locked, 0 - not locked, error code if negative).
sts_t lock(void);

/// Unlock the RTOS Kernel scheduler.
/// \return previous lock state (1 - locked, 0 - not locked, error code if negative).
sts_t unlock(void);

/// Restore the RTOS Kernel scheduler lock state.
/// \param[in]     lock          lock state obtained by \ref osKernelLock or \ref osKernelUnlock.
/// \return new lock state (1 - locked, 0 - not locked, error code if negative).
sts_t restore_lock(const sts_t _lock);

/// Suspend the RTOS Kernel scheduler.
/// \return time in ticks, for how long the system can sleep or power-down.
uint32_t suspend(void);

/// Resume the RTOS Kernel scheduler.
/// \param[in]     sleep_ticks   time in ticks for how long the system was in sleep or power-down mode.
void resume(const uint32_t _sleep_ticks);

/// Get the RTOS kernel tick count.
/// \return RTOS kernel current tick count.
uint32_t get_tick_count(void);

/// Get the RTOS kernel tick frequency.
/// \return frequency of the kernel tick in hertz, i.e. kernel ticks per second.
uint32_t get_tick_freq(void);

/// Get the RTOS kernel system timer count.
/// \return RTOS kernel current system timer count as 32-bit value.
uint32_t get_sys_timer_count(void);

/// Get the RTOS kernel system timer frequency.
/// \return frequency of the system timer in hertz, i.e. timer ticks per second.
uint32_t get_sys_timer_freq(void);

}; // namespace kernel

} // namespace os
