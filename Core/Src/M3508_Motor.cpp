//
// Created by lenovo on 24-10-25.
//
#include "M3508_Motor.h"

float linearMapping(int in, int in_min, int in_max, float out_min, float out_max){
    if (in_min == in_max){
        return out_min;
    }
    else{
        float out = out_min + (out_max - out_min) / (in_max - in_min) * (in - in_min);
        return out;
    }
}

M3508_Motor::M3508_Motor() {
    ratio_ = 3591.0/187;
    angle_ = 0;
    delta_angle_ = 0;
    ecd_angle_ = 0;
    last_ecd_angle_ = 0;
    delta_ecd_angle_ = 0 ;
    rotate_speed_ = 482.0*360/60; //dps 计算得1 rpm = 6 dps
    current_ = 0.78; //A
    temp_ = 25; //°C
}

void M3508_Motor::CanRxMsgCallback(uint8_t *rx_data) {
    uint16_t datau16[3]; //创建16位整型， 用于将高8位和低8位合并
    int16_t data16[3];
    datau16[0] = (rx_data[0] << 8) | rx_data[1];
    datau16[1] = (rx_data[2] << 8) | rx_data[3];
    datau16[2] = (rx_data[4] << 8) | rx_data[5]; //合并
    ecd_angle_ = linearMapping(datau16[0], 0, 8191, 0, 360); //机械角度转换
    rotate_speed_ = datau16[1]*6; //转速值转换（需将rpm转换为dps）
    data16[2] = static_cast<int16_t>(datau16[2]); // 转换为 int16_t 类型,使其可以表示负整数
    current_ = linearMapping(data16[2], -16384, 16383, -20, 20); //电流转换
    temp_ = rx_data[6];//温度

    delta_ecd_angle_ = ecd_angle_ - last_ecd_angle_;//作差求出变化量，运动角度在-180°~180°之间。
    last_ecd_angle_ = ecd_angle_;

    delta_angle_ = delta_ecd_angle_ / ratio_ ; //输出轴角度需要除以减速比
    angle_ = angle_ + delta_angle_ ;
}