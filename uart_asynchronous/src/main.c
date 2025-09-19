#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

/* USART2 peripheral insatance */
const struct device *usart2 = DEVICE_DT_GET(DT_NODELABEL(usart2));
static uint8_t rx_buffer[20] = {0}; /*buffer for receiving the data*/
static uint8_t tx_buffer[] = "HELLO TRANSMITTNG DATA\r\n";
volatile bool dataReceived = false;


/* config details of USART2 */
const struct uart_config usart2_config = { 
    .baudrate  = 115200,
    .parity    = UART_CFG_PARITY_NONE,
    .stop_bits = UART_CFG_STOP_BITS_1,
    .data_bits = UART_CFG_DATA_BITS_8,
    .flow_ctrl = UART_CFG_FLOW_CTRL_NONE
};

/* function declarations */
static int usart2_init(void);
static void uartCallback(const struct device *dev, struct uart_event *evt, void *user_data);
static void enable_usart2Receiving(void);


int main()
{
    usart2_init();
    enable_usart2Receiving();

    while(1)
    {
       int error = uart_tx(usart2,tx_buffer,sizeof(tx_buffer),200);
       if(error)
       {
        return -1;
       }
        k_msleep(1000);
        if(dataReceived == true)
        {
            error = uart_tx(usart2,rx_buffer,sizeof(rx_buffer),200);
            if(error)
            {
                return -1;
            }
            dataReceived = false; 
        }
 
    }
    return 0;
}

static int usart2_init(void)
{
    if(!device_is_ready(usart2))
    {
        return -1;
    }
    uart_configure(usart2,&usart2_config);
    int error = uart_callback_set(usart2, &uartCallback, NULL);
    if(error)
    {
        return error;
    }
    return 0;
}

/** @brief UART callback
*  @param dev : device instance
*  @param evt : event details
*  @return None
*/
static void uartCallback(const struct device *dev, struct uart_event *evt, void *user_data)
{
    switch (evt->type)
    {
    /* Whole TX buffer was transmitted. */
    case UART_TX_DONE:
        break;
    /*Transmitting aborted due to timeout or uart_tx_abort call*/
    case UART_TX_ABORTED:
        break;
    /*Received data is ready for processing.*/
    case UART_RX_RDY:
        dataReceived = true;
        break;
    /*Driver requests next buffer for continuous reception*/
    case UART_RX_BUF_REQUEST:
        break;
    /*Buffer is no longer used by UART driver.*/
    case UART_RX_BUF_RELEASED:
        break;
    /*RX has been disabled and can be reenabled.*/
    case UART_RX_DISABLED:
        dataReceived = true;
        enable_usart2Receiving();
        break;
    /*RX has stopped due to external event.*/
    case UART_RX_STOPPED:
        break;    
    default:
        break;
    }
}

/**
 * @brief function to enable the receiving
 * @param  :None
 * @return :None
 */
static void enable_usart2Receiving(void)
{
    uart_rx_enable(usart2,rx_buffer,sizeof(rx_buffer),200);
}