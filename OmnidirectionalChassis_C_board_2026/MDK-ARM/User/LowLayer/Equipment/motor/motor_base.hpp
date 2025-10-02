#ifndef _MOTOR_BASE_HPP_
#define _MOTOR_BASE_HPP_ 

#include "can_driver.hpp"
#include <array>
#include <memory>
#include <unordered_map>
#include "../User/MidLayer/Managers/state_manager/state_base.hpp"
#include "../User/MidLayer/Algorithms/odometry/odometry.hpp"

namespace motor
{
    template <uint8_t N> class MotorBase : public State::monitoring
    {
        protected:
            struct EncoderData
            {
                int16_t ref_angle;              //ԭʼ�Ƕ�
                int16_t ref_speed;              //ԭʼ�ٶ�
                int16_t ref_torqueCurrent;      //ԭʼ����
                int16_t ref_torque;             //ԭʼŤ��
                uint8_t ref_temperate;          //ԭʼ�¶�

                float angle_deg;                //ת�ӽǶ� �Ƕ�
                float angle_rad;                //ת�ӽǶ� ����            
                float speed_rpm;                //ת���ٶ� rpm
                float speed_rad_s;              //ת���ٶ� ����/s
                float torquecurrent;            //ʵ��Ť�ص��� A
                float torque;                   //���Ť�� nm

                float add_angle;

                bool is_Online;
                float target;

                // ���캯����ʼ�����г�Ա
                EncoderData() : 
                    ref_angle(0), ref_speed(0), ref_torqueCurrent(0), ref_torque(0), ref_temperate(0),
                    angle_deg(0.0f), angle_rad(0.0f), speed_rpm(0.0f), speed_rad_s(0.0f),
                    torquecurrent(0.0f), torque(0.0f), add_angle(0.0f), is_Online(false), target(0.0f) {}
            };

            std::array<EncoderData, N> encoderdata;
        
        public:
            float GetSpeedRef(uint8_t id)
            {
                return encoderdata[id-1].ref_speed;
            }

            flloat GetAngleRef(uint8_t id)
            {
                return encoderdata[id-1].ref_angle;
            }

            float GetTorqueCurrentRef(uint8_t id)
            {
                return encoderdata[id-1].ref_torqueCurrent;
            }

            float GetTorqueRef(uint8_t id)
            {
                return encoderdata[id-1].ref_torque;
            }

            float GetTemperatureRef(uint8_t id)
            {
                return encoderdata[id-1].ref_temperate;
            }

            float GetAngleDeg(uint8_t id)
            {
                return encoderdata[id-1].angle_deg;
            }

            float GetAngleRad(uint8_t id)
            {
                return encoderdata[id-1].angle_rad;
            }

            float GetSpeedRpm(uint8_t id)
            {
                return encoderdata[id-1].speed_rpm;
            }

            float GetSpeedRadS(uint8_t id)
            {
                return encoderdata[id-1].speed_rad_s;
            }

            float GetTorquecurrent_A(uint8_t id)
            {
                return encoderdata[id-1].torquecurrent;
            }

            float GetTorqueNm(uint8_t id)
            {
                return encoderdata[id-1].torque;
            }

    };
}


#endif
