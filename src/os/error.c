
#include "cmsis_compiler.h"
#include "rtx_os.h"
#include "misc.h"

/// OS Error Callback function
extern uint32_t osRtxErrorNotify(uint32_t code, void *object_id);
uint32_t osRtxErrorNotify(uint32_t code, void *object_id)
{
    uint32_t id = (uint32_t)(object_id);
    switch (code)
    {
        case osRtxErrorStackOverflow:
        {
            // Stack overflow detected for thread (thread_id=object_id)
            const char *name = osThreadGetName(object_id);
            printerr("Stack overflow detected for thread. ID = %#010x. Name = '%s'.\n", id, (name != NULL) ? name : "undefined");
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
            printerr("User Timer Callback Queue overflow detected for timer. ID = %#010x. Name = '%s'.\n", id, (name != NULL) ? name : "undefined");
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
    for (;;);
}

/**
 * @brief Структура регистров на момент HARDFAULT
 */
struct reg_dump_t
{
    uint32_t r0;    //!< Значение регистра R0  в момент генерации HARDFAULT
    uint32_t r1;    //!< Значение регистра R1  в момент генерации HARDFAULT
    uint32_t r2;    //!< Значение регистра R2  в момент генерации HARDFAULT
    uint32_t r3;    //!< Значение регистра R3  в момент генерации HARDFAULT
    uint32_t r12;   //!< Значение регистра R12 в момент генерации HARDFAULT
    uint32_t lr;    //!< Значение регистра LR  в момент генерации HARDFAULT
    uint32_t pc;    //!< Значение регистра PC  в момент генерации HARDFAULT
    uint32_t psr;   //!< Значение регистра PSR в момент генерации HARDFAULT
};

__USED static void _reg_dump(uint32_t *stack_addr);

/**
 * @brief Запись состояние регистров в лог (запись HardFault) до входа в прерывание
 * @warning Замечено что если функция _system_log_registers располагается выше по тексту данной функции, то в регистр \n
 *          r2 в строке "ldr r2, handler2_address_const" записывается неверное значение адреса функции, и дальнейшая инструкция
 *          "bx r2" улетает за пределы Flash - соотв. записи в лог не происходит
 */
__STATIC_INLINE void reg_dump(void)
{
    __asm volatile
    (
        " tst lr, #4                    \n"
        " ite eq                        \n"
        " mrseq r0, msp                 \n"
        " mrsne r0, psp                 \n"
        " ldr r1, [r0, #24]             \n"
        " ldr r2, reg_dump_ptr          \n"
        " bx r2                         \n"
        " reg_dump_ptr: .word _reg_dump \n"
    );
}

/**
 * @brief Генерация сообщения HardFault и запись в лог
 * @param[in] stack_addr Указатель на стек до вызова прерывания
 */
static void _reg_dump(uint32_t *stack_addr)
{
    const struct reg_dump_t hf =
    {
        .r0  = stack_addr[0],
        .r1  = stack_addr[1],
        .r2  = stack_addr[2],
        .r3  = stack_addr[3],
        .r12 = stack_addr[4],
        .lr  = stack_addr[5], /* Link register. */
        .pc  = stack_addr[6], /* Program counter. */
        .psr = stack_addr[7], /* Program status register. */
    };
    
    printerr("Hard fault.\n");
    printerr("Regisers dump:\n");
    printf("\t r0  = %#010x\n", hf.r0 );
    printf("\t r1  = %#010x\n", hf.r1 );
    printf("\t r2  = %#010x\n", hf.r2 );
    printf("\t r3  = %#010x\n", hf.r3 );
    printf("\t r12 = %#010x\n", hf.r12);
    printf("\t lr  = %#010x\n", hf.lr );
    printf("\t pc  = %#010x\n", hf.pc );
    printf("\t psr = %#010x\n", hf.psr);
}

/// System hard fault
extern void HardFault_Handler(void);
__NO_RETURN void HardFault_Handler(void)
{
    reg_dump();
    for (;;);
}
