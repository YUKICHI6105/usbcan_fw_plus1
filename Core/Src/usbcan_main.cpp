#include "main.h"
#include "usbd_cdc_if.h"
#include "usb_device.h"
#include "led.h"
#include "can_usb.h"
#include "buffer.hpp"

extern CAN_HandleTypeDef hcan;
extern USBD_HandleTypeDef hUsbDeviceFS;
extern Buffer buffer;

void main_cpp()
{
	HAL_CAN_Start(&hcan);

	uint8_t debug_state = 0;
	while (true)
	{
		if (READ_BIT(CAN->MSR, CAN_MSR_INAK) == 1)
		{
			// can start
			CLEAR_BIT(CAN->MCR, CAN_MCR_INRQ);
			led_on(green);
		}
		if (debug_state == 0)
		{
			switch (hUsbDeviceFS.dev_state)
			{
			case USBD_STATE_DEFAULT:
				led_on(red);
				break;
			case USBD_STATE_ADDRESSED:
				led_on(yellow);
				break;
			case USBD_STATE_SUSPENDED:
				led_on(yellow);
				led_on(red);
				break;
			case USBD_STATE_CONFIGURED:
				led_on(green);
				debug_state = 1;
				break;
			default:
				break;
			}
		}
		else if (debug_state == 1)
		{
			switch (hcan.ErrorCode)
			{
			case HAL_CAN_ERROR_BD: // bit dominant error
				led_on(red);
				break;
			case HAL_CAN_ERROR_ACK: // acknowledge error
				led_on(yellow);
				break;
			default:
				break;
			}
		}
		buffer.resend(hcan);
		led_process();
	}
}
