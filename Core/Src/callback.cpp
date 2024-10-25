//
// Created by lenovo on 24-10-25.
//
#include "can.h"
#include "main.h"
#include "M3508_Motor.h"

extern CAN_RxHeaderTypeDef RxHeader;
extern uint8_t rx_data[8];
extern M3508_Motor Motor;

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    if (hcan->Instance==CAN1){
        HAL_CAN_GetRxMessage(&hcan1, CAN_FilterFIFO0, &RxHeader, rx_data);
        Motor.CanRxMsgCallback(rx_data);
    }
}