/**
 * @note use misc control
 * -Wno-c++98-compat -Wno-c++98-compat-pedantic
 * -Wno-global-constructors -Wno-missing-variable-declarations
 * -Wno-gnu-zero-variadic-macro-arguments
 *
 * @note mutexes in the standard library, which are created via @ref _mutex_initialize
 * see https://developer.arm.com/documentation/dui0475/m/the-c-and-c---library-functions-reference/thread-safe-c-library-functions
 * 1 - heap operations
 * 2 - 
 * 3 - standard in out operation, etc printf
 * 4 -
 * 5 - file operations, etc fopen, fclose
 * x - A mutex is created for each open file. Mutex is reused
 */

#ifdef   _RTE_
    #include "RTE_Components.h"
    #include CMSIS_device_header
#endif

#include <stdint.h>
#include <stdio.h>

#include "rtx_os.h"

struct arr
{
    uint32_t arr[256];
};

static arr array __attribute__((section("user.section")));

class arr_exploiter
{
protected:
    arr &arr_;
public:
    constexpr arr_exploiter(arr &_arr): arr_(_arr){}
    void runer()
    {
        for (auto &i : arr_.arr) i = 0x55;
        printf("this addr: %#010x arr addr: %#010x\n", reinterpret_cast<uint32_t>(this), reinterpret_cast<uint32_t>(&arr_));
    }
};

template <arr_exploiter &_exp> class arr_exploiter_wrapper: public arr_exploiter
{
private:
    inline static arr array_ __attribute__((section("user.section")));
public:
    constexpr arr_exploiter_wrapper():
        arr_exploiter(array_)
    {
    }
};

static arr_exploiter exploiter0 = array;

static arr_exploiter exploiter2 = arr_exploiter_wrapper<exploiter2>();
static arr_exploiter exploiter3 = arr_exploiter_wrapper<exploiter3>();

int main()
{
    NVIC_SetPriorityGrouping(3);
    SystemCoreClockUpdate();

    printf("\033[31mC\033[32mO\033[33mL\033[34mO\033[35mR\033[42m \033[0m \033[36mT\033[37mE\033[30m\033[47mS\033[0mT\n"); // Color test

    exploiter0.runer();
    exploiter2.runer();
    exploiter3.runer();
    
    osKernelInitialize();
    // create some threads
    osKernelStart();
    
    printf("Error: kernel not started.\n");
}
