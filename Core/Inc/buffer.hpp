#include <main.h>
#include <led.h>

struct Data{
	uint8_t data[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	bool sended = true;
};

class Buffer{
private:
	CAN_TxHeaderTypeDef TxHeader;
	uint32_t TxMailbox;
	Data buffer[4];
public:
	void setBuffer(uint8_t usb_msg[]);
	void resend(CAN_HandleTypeDef& hcan);
};

inline void Buffer::setBuffer(uint8_t usb_msg[]){
	for(uint8_t j = 0;j<4;j++){
		if(buffer[j].sended == true){
			for(uint8_t i = 0;i<8;i++){
				buffer[j].data[i] = usb_msg[i];
			}
			buffer[j].sended = false;
		}
	}
}

inline void Buffer::resend(CAN_HandleTypeDef& hcan){
	for(uint8_t i = 0;i<4;i++){
		if(buffer[i].sended == false){
			if (buffer[i].data[0] & 0x02)
			{
		        // extended id
		        TxHeader.IDE = CAN_ID_EXT;
		        TxHeader.ExtId = (buffer[i].data[1] << 24) | (buffer[i].data[2] << 16) | (buffer[i].data[3] << 8) | (buffer[i].data[4] << 0);
		    }
		    else
		    {
		        // standard id
		        TxHeader.IDE = CAN_ID_STD;
		        TxHeader.StdId = (buffer[i].data[1] << 24) | (buffer[i].data[2] << 16) | (buffer[i].data[3] << 8) | (buffer[i].data[4] << 0);
		    }

		    // is_rtr
		    if (buffer[i].data[0] & 0x04)
		    {
		        TxHeader.RTR = CAN_RTR_REMOTE;
		    }
		    else
		    {
		        TxHeader.RTR = CAN_RTR_DATA;
		    }

		    // is_error
		    //?

		    TxHeader.TransmitGlobalTime = DISABLE;
		    // dlc
		    TxHeader.DLC = buffer[i].data[5];

		    if (0 < HAL_CAN_GetTxMailboxesFreeLevel(&hcan))
		    {
		        led_on(can);
		        HAL_CAN_AddTxMessage(&hcan, &TxHeader, buffer[i].data + 6, &TxMailbox);
		        buffer[i].sended = true;
		    }
		}
	}
}

