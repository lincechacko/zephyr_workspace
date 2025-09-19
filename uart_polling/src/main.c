#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

#define UART_DEVICE_NODE DT_NODELABEL(usart2)

const struct device *uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE); 

/* @brief  config details 
 * modify to change the configs 
*/
struct uart_config uart_cfg = {    
	.baudrate = 9600,
	.parity = UART_CFG_PARITY_NONE,
	.stop_bits = UART_CFG_STOP_BITS_1,
	.flow_ctrl = UART_CFG_FLOW_CTRL_NONE,
	.data_bits = UART_CFG_DATA_BITS_8,
};

int main()
{
    char hello [] = "hello world\r\n";   /*text to be transmitted through uart*/
    uart_configure(uart_dev,&uart_cfg);  /* API to configure the UART */

    if (!device_is_ready(uart_dev)) {    /* check uart init success */
        return -1;
    }

    while (1)
    {
        for(int i =0;i<sizeof(hello);i++)
        {
            uart_poll_out(uart_dev, hello[i]); /*transmitting the data through uart in polling*/
        }

        k_msleep(1000);
    }
    
    return 0;
}