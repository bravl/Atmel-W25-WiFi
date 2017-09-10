/************************************************************************/
/* Author: BRAVL                                                        */
/* WiFi test                                                            */
/************************************************************************/

#include <asf.h>

#define SERVER_IP 0xc0a80035
#define SERVER_PORT 1234

#include "common/include/nm_common.h"
#include "driver/include/m2m_wifi.h"
#include "socket/include/socket.h"

#define MAIN_WLAN_SSID                    "WiFi-Here" /**< Destination SSID */
#define MAIN_WLAN_AUTH                    M2M_WIFI_SEC_WPA_PSK /**< Security manner */
#define MAIN_WLAN_PSK                     "passwd-here" /**< Password for Destination SSID */

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

static void wifi_callback(uint8_t u8MsgType, void *pvMsg)
{
	printf("WiFi callback triggered\n");
}

int main (void)
{
	int ret = 0;
	tstrWifiInitParam param;
	struct sockaddr_in addr;
	
	system_init();
	nm_bsp_init();
	configure_console();
	
	printf("Running WiFi BSP\n");
	
	addr.sin_family = AF_INET;
	addr.sin_port = _htons(SERVER_PORT);
	addr.sin_addr.s_addr = _htonl(SERVER_IP);

	memset((uint8_t *)&param, 0, sizeof(tstrWifiInitParam));

	param.pfAppWifiCb = wifi_callback;
	ret = m2m_wifi_init(&param);
	if (M2M_SUCCESS != ret) {
		printf("Init callback failed\n");
		while (1) {
		}
	}

	printf("Connecting to WiFi network\n");
	ret = m2m_wifi_connect((char*)MAIN_WLAN_SSID,sizeof(MAIN_WLAN_SSID),MAIN_WLAN_AUTH,(char*)MAIN_WLAN_PSK,M2M_WIFI_CH_ALL);
	if (M2M_SUCCESS != ret) {
		printf("Connect failed\n");
		while (1) {
		}
	}
	printf("Connected to WiFi network");

	while (1) {
		if (port_pin_get_input_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
			printf("Button pressed\n");
			port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
		} else {
			port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);
		}
	}
}
