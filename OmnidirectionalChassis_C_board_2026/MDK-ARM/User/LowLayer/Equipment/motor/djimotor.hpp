#ifndef _DJIMOTOR_HPP_
#define _DJIMOTOR_HPP_

#include "../User/LowLayer/HAL_/can/can_driver.hpp"



// namespace motor
// {
//     struct AngleCalculator 
//     {
//         float maxAngle;           // ���Ƕ�ֵ
//         float accumulatedAngle;   // �ۼƽǶ�
//         float lastAngleRaw;       // ��һ��ԭʼ�Ƕ�ֵ
//         bool initialized;         // ��ʼ����־
        
//         AngleCalculator(float maxAngleValue) : 
//             maxAngle(maxAngleValue), 
//             accumulatedAngle(0.0f), 
//             lastAngleRaw(0.0f), 
//             initialized(false) {}
        
//         // �����ۼƽǶ�
//         float update(float currentRawAngle) 
//         {
//             if (!initialized) 
//             {
//                 lastAngleRaw = currentRawAngle;
//                 accumulatedAngle = currentRawAngle;
//                 initialized = true;
//                 return accumulatedAngle;
//             }
            
//             // ����Ƕȷ�ת���
//             float delta = currentRawAngle - lastAngleRaw;
//             const float halfMax = maxAngle / 2.0f;
            
//             if (delta > halfMax) 
//             {
//                 // ���·�ת
//                 accumulatedAngle += (-maxAngle + delta);
//             } 
//             else if (delta < -halfMax) 
//             {
//                 // ���Ϸ�ת
//                 accumulatedAngle += (maxAngle + delta);
//             }
//             else 
//             {
//                 // �����仯
//                 accumulatedAngle += delta;
//             }
            
//             lastAngleRaw = currentRawAngle;
//             return accumulatedAngle;
//         }
//     };

//     class DJImotor : public State::monitoring
//     {   
//         public:
//             DJImotor(uint32_t recv_id) { recv_id_ = recv_id; };
//             void DataUpdate(const uint8_t *data, float Maximum_angle);

//             float getAccumulatedAngle(float maxAngleValue) 
//             {
//                 // �����Ƿ���ڶ�Ӧ�ĽǶȼ�����
//                 for (auto& calc : angleCalculators) 
//                 {
//                     if (calc.maxAngle == maxAngleValue) 
//                     {
//                         return calc.update((float)angle);
//                     }
//                 }
                
//                 // ��������ڣ��򴴽��µĽǶȼ�����
//                 angleCalculators.emplace_back(maxAngleValue);
//                 return angleCalculators.back().update((float)angle);
//             }

//             float totalAngle;               //�ۼƽǶ�
//             float MotorSet;                 //�������ֵ
        
//         private:
//             uint32_t recv_id_;              //����id
//             int16_t ref_angle;              //ת�ӻ�е�Ƕ�  0-8191
//             int16_t ref_speed;              //ת���ٶ�      rpm
//             int16_t ref_torqueCurrent;      //ʵ��Ť�ص���
//             uint8_t ref_temperate;          //����¶�      ���϶�

//             float angle;                    //ת�ӻ�е�Ƕ�,float              
//             float speed;                    //ת���ٶ�,float        
//             float torquecurrent;            //ʵ��Ť�ص���,float
//             float torque;                   //���Ť��

//             float angle_8191;               //ת�ӻ�е�Ƕ�  0-8191
//             float angle_360;                //ת�ӻ�е�Ƕ�  0-360
//             float speed_rad;                //ת���ٶ�      rad/s

//             //�ṩ�����ӿڣ���������Ƕȵ�ֵ��ǰ����ǽṹ�壬������һЩ�����ۼӽǶȵı����ͺ���
//             AngleCalculator angle8191Calc{8191.0f};
//             AngleCalculator angle360Calc {360.0f };
//             std::vector<AngleCalculator> angleCalculators;
//     };

//     class DJIMotorController 
//     { 
//     public:
//         explicit DJIMotorController(CanDriver::CanHal& can) : can_(can) {}
    
//         // ��������API����
//         bool Send0x200(int16_t m1, int16_t m2, int16_t m3, int16_t m4) //ֵ��������������
//         {
//             return SendMotorData(0x200, m1, m2, m3, m4);
//         }
        
//         bool Send0x1FF(int16_t m1, int16_t m2, int16_t m3, int16_t m4) //ֵ��������������
//         {
//             return SendMotorData(0x1FF, m1, m2, m3, m4);
//         }

//         bool Send0x1FE(int16_t m1, int16_t m2, int16_t m3, int16_t m4) //ֵ��������������
//         {
//             return SendMotorData(0x1FE, m1, m2, m3, m4);
//         }

//     private:
//         bool SendMotorData(uint16_t cmdId, int16_t m1, int16_t m2, int16_t m3, int16_t m4); //����֡������
//         CanDriver::CanHal& can_;
        
//         static void PackInt16(uint8_t* dst, int16_t val) //���ݴ�����
//         {
//             dst[0] = static_cast<uint8_t>(val >> 8);
//             dst[1] = static_cast<uint8_t>(val & 0xFF);
//         };
//     };
// }

// void RmMotorRxData(const CanDriver::CanFrame& frame);
namespace motor
{
    struct Coefficient
    {
        double reduction_ratio;      // ���ٱ�
        double torque_constant;      // ���س��� (Nm/A)
        double feedback_current_max; // ���������� (A)
        double current_max;          // ������ (A)
        double encoder_resolution;   // �������ֱ���
        double max_angle;            // ���Ƕ�

        // �Զ�����Ĳ���
        double encoder_to_deg;                  // ������ֵת�Ƕ�ϵ��
        double encoder_to_rpm;                  // ������ֵת�ٶ�ϵ��
        double rpm_to_radps;                    // RPMת���ٶ�ϵ��
        double current_to_torque_coefficient;   // ����תŤ��ϵ��
        double feedback_to_current_coefficient; // ��������ת����ϵ��
        double deg_to_real;                     // �Ƕ�תʵ�ʽǶ�ϵ��

        static constexpr double deg_to_rad = 0.017453292519611;
        static constexpr double rad_to_deg = 1 / 0.017453292519611;
            
        // ���캯������������ rr���ٱȣ�tcŤ�س�����fmc������������mc��������er�������ֱ��� ma���Ƕȣ�һȦ�����ӽ��ã�
        Coefficient (double rr, double tc, double fmc, double mc, double er, double ma)
            : reduction_ratio(rr), torque_constant(tc), feedback_current_max(fmc), current_max(mc), 
            encoder_resolution(er), max_angle(ma)
        {
            constexpr double PI = 3.14159265358979323846;
            encoder_to_deg = 360.0 / encoder_resolution;
            rpm_to_radps = 1 / reduction_ratio / 60 * 2 * PI;
            encoder_to_rpm = 1 / reduction_ratio;
            current_to_torque_coefficient = reduction_ratio * torque_constant / feedback_current_max * current_max;
            feedback_to_current_coefficient = current_max / feedback_current_max;
            deg_to_real = 1 / reduction_ratio;
        }
    };

    template<uint8_t N> class DJImotorBase : public motor_base<N>
    {
        public:
            DJImotorBase(uint16_t init_id_, const uint8_t(*rxid_)[N], uint32_t txid_, Coefficient* data)
            {
                for(uint8_t i = 0; i < N; ++i)
                {
                    rxid[i] = rxid_[i];
                }
                txid = txid_;
                init_id = init_id_;
                CoefficientData = data;
            }

            Coefficient CoefficientData;

            void DataUpdate(const uint8_t *data, uint8_t index)
            {
                this->encoderdata[index].ref_angle          = (int16_t)(data[0]<<8|data[1]);     //�Ƕ�ֵ
                this->encoderdata[index].ref_speed          = (int16_t)(data[2]<<8|data[3]);     //�ٶ�
                this->encoderdata[index].ref_torqueCurrent  = (int16_t)(data[4]<<8|data[5]);     //����
                this->encoderdata[index].ref_temperate      = data[6];                           //�¶�
            }

            void RmMotorRxData(const CanDriver::CanFrame& frame)
            {
                const uint16_t receive_id = frame.id;

                for(uint8_t i = 0; i < N; ++i)
                {
                    if(receive_id == init_id + rxid[i])
                    {
                        // ��ȡ�������
                        DataUpdate(frame.data, i);

                    }
                }
            }

            void Configure(size_t i)
            {
                this->encoderdata[i].angle_deg = this->encoderdata[i].ref_angle * CoefficientData->encoder_to_deg;
                this->encoderdata[i].angle_rad = this->encoderdata[i].angle_deg * CoefficientData->deg_to_rad;
                this->encoderdata[i].speed_rpm = this->encoderdata[i].ref_speed * CoefficientData->encoder_to_rpm;
                this->encoderdata[i].speed_rad = this->encoderdata[i].speed_rpm * CoefficientData->rpm_to_radps;
                this->encoderdata[i].torque_Nm = this->encoderdata[i].ref_torqueCurrent * CoefficientData->current_to_torque_coefficient;
                this->encoderdata[i].current_A = this->encoderdata[i].ref_torqueCurrent * CoefficientData->feedback_to_current_coefficient;
                this->encoderdata[i].temperate_C = this->encoderdata[i].ref_temperate;

                this->encoderdata[i].add_angle = 
                
            }


        private:
            uint8_t rxid[N];
            uint32_t txid;
            uint16_t init_id


    };

}

#ifdef __cplusplus
extern "C" {
#endif
	
void CanInit();

#ifdef __cplusplus
}
#endif

#endif
