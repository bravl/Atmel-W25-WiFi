/************************************************************************/
/* Author: BRAVL                                                        */
/* WiFi test                                                            */
/************************************************************************/

#include <asf.h>

#include "common/include/nm_common.h"
#include "driver/include/m2m_wifi.h"
#include "socket/include/socket.h"

static struct usart_module cdc_uart_module;

static void configure_console(void)
{
	struct usart_config usart_conf;

	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	usart_conf.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	usart_conf.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	usart_conf.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	usart_conf.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
	usart_conf.baudrate    = 115200;

	stdio_serial_init(&cdc_uart_module, EDBG_CDC_MODULE, &usart_conf);
	usart_enable(&cdc_uart_module);
}

int main (void)
{
	system_init();
	configure_console();
	
	printf("Initializing WiFi BSP\n");
	nm_bsp_init();
	
	while (1) {
		if (port_pin_get_input_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
			printf("Button pressed\n");
			port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
		} else {
			port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);
		}
	}
}
