#ifndef _DJIMOTOR_HPP_
#define _DJIMOTOR_HPP_

#include "../User/LowLayer/HAL_/can/can_driver.hpp"
#include "../User/MidLayer/Managers/state_manager/state.hpp"

#define MY_PI 3.1415926535897932384626433832795

namespace motor
{
    struct AngleCalculator 
    {
        float maxAngle;           // ���Ƕ�ֵ
        float accumulatedAngle;   // �ۼƽǶ�
        float lastAngleRaw;       // ��һ��ԭʼ�Ƕ�ֵ
        bool initialized;         // ��ʼ����־
        
        AngleCalculator(float maxAngleValue) : 
            maxAngle(maxAngleValue), 
            accumulatedAngle(0.0f), 
            lastAngleRaw(0.0f), 
            initialized(false) {}
        
        // �����ۼƽǶ�
        float update(float currentRawAngle) 
        {
            if (!initialized) 
            {
                lastAngleRaw = currentRawAngle;
                accumulatedAngle = currentRawAngle;
                initialized = true;
                return accumulatedAngle;
            }
            
            // ����Ƕȷ�ת���
            float delta = currentRawAngle - lastAngleRaw;
            const float halfMax = maxAngle / 2.0f;
            
            if (delta > halfMax) 
            {
                // ���·�ת
                accumulatedAngle += (-maxAngle + delta);
            } 
            else if (delta < -halfMax) 
            {
                // ���Ϸ�ת
                accumulatedAngle += (maxAngle + delta);
            }
            else 
            {
                // �����仯
                accumulatedAngle += delta;
            }
            
            lastAngleRaw = currentRawAngle;
            return accumulatedAngle;
        }
    };

    class DJImotor : public State::monitoring
    {   
        public:
            DJImotor(uint32_t recv_id) { recv_id_ = recv_id; };
            void DataUpdate(const uint8_t *data, float Maximum_angle);

            float getAccumulatedAngle(float maxAngleValue) 
            {
                // �����Ƿ���ڶ�Ӧ�ĽǶȼ�����
                for (auto& calc : angleCalculators) 
                {
                    if (calc.maxAngle == maxAngleValue) 
                    {
                        return calc.update((float)angle);
                    }
                }
                
                // ��������ڣ��򴴽��µĽǶȼ�����
                angleCalculators.emplace_back(maxAngleValue);
                return angleCalculators.back().update((float)angle);
            }

            float totalAngle;               //�ۼƽǶ�
            float MotorSet;                 //�������ֵ
        
        private:
            uint32_t recv_id_;              //����id
            int16_t ref_angle;              //ת�ӻ�е�Ƕ�  0-8191
            int16_t ref_speed;              //ת���ٶ�      rpm
            int16_t ref_torqueCurrent;      //ʵ��Ť�ص���
            uint8_t ref_temperate;          //����¶�      ���϶�

            float angle;                    //ת�ӻ�е�Ƕ�,float              
            float speed;                    //ת���ٶ�,float        
            float torquecurrent;            //ʵ��Ť�ص���,float
            float torque;                   //���Ť��

            float angle_8191;               //ת�ӻ�е�Ƕ�  0-8191
            float angle_360;                //ת�ӻ�е�Ƕ�  0-360
            float speed_rad;                //ת���ٶ�      rad/s

            //�ṩ�����ӿڣ���������Ƕȵ�ֵ��ǰ����ǽṹ�壬������һЩ�����ۼӽǶȵı����ͺ���
            AngleCalculator angle8191Calc{8191.0f};
            AngleCalculator angle360Calc {360.0f };
            std::vector<AngleCalculator> angleCalculators;
    };

    class DJIMotorController 
    { 
    public:
        explicit DJIMotorController(CanDriver::CanHal& can) : can_(can) {}
    
        // ��������API����
        bool Send0x200(int16_t m1, int16_t m2, int16_t m3, int16_t m4) //ֵ��������������
        {
            return SendMotorData(0x200, m1, m2, m3, m4);
        }
        
        bool Send0x1FF(int16_t m1, int16_t m2, int16_t m3, int16_t m4) //ֵ��������������
        {
            return SendMotorData(0x1FF, m1, m2, m3, m4);
        }

        bool Send0x1FE(int16_t m1, int16_t m2, int16_t m3, int16_t m4) //ֵ��������������
        {
            return SendMotorData(0x1FE, m1, m2, m3, m4);
        }

    private:
        bool SendMotorData(uint16_t cmdId, int16_t m1, int16_t m2, int16_t m3, int16_t m4); //����֡������
        CanDriver::CanHal& can_;
        
        static void PackInt16(uint8_t* dst, int16_t val) //���ݴ�����
        {
            dst[0] = static_cast<uint8_t>(val >> 8);
            dst[1] = static_cast<uint8_t>(val & 0xFF);
        };
    };
}

void RmMotorRxData(const CanDriver::CanFrame& frame);

#ifdef __cplusplus
extern "C" {
#endif
	
void CanInit();

#ifdef __cplusplus
}
#endif

#endif
