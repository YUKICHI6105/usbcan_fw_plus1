/*
 * user_if.c
 *
 *  Created on: Dec 26, 2022
 *      Author: taman
 */

#include "main.h"
#include "can_usb.h"
#include "usbd_cdc_if.h"

#include "led.h"

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    // data structure
    /*
    uint8_t command & frame_type: (command: if it is normal can frame, it is 0x00.)<<4 | is_rtr << 2 | is_extended << 1 | is_error
    uint8_t id[4] : can id
    uint8_t dlc : data length
    uint8_t data[8] : data (it is pre-writtten.)
    */
    uint8_t Data[14];
    CAN_RxHeaderTypeDef RxHeader;
    // the Data is used for USB buffer. can_process set header infomation to Data[0~5].
    // It is a terrible code. Sorry for hard work to read the code.
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, Data + 6) == HAL_OK)
    {

        can_process(&RxHeader, Data);
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin ==EMS_Pin){
		led_on(red);
		if(HAL_GPIO_ReadPin(EMS_GPIO_Port, EMS_Pin)){
	        static uint8_t HelloSLCAN_encoded[] = {0x03, 0x02 << 4,'H', 0x00};
	        CDC_Transmit_FS(HelloSLCAN_encoded, 2 + 2);
		}else{
	        static uint8_t HelloSLCAN_encoded[] = {0x03, 0x02 << 4,'L', 0x00};
	        CDC_Transmit_FS(HelloSLCAN_encoded, 2 + 2);
		}
	}
}
