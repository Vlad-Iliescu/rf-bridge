#ifndef P1P_RESET_TASK_H
#define P1P_RESET_TASK_H
#ifdef __cplusplus
extern "C" {
#endif

#include <driver/gpio.h>

/**
 * Checks if the reset button was pressed
 * @param unused
 */
void init_factory_reset_task(gpio_num_t reset_pin);

#ifdef __cplusplus
}
#endif
#endif //P1P_RESET_TASK_H
