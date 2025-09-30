#ifndef _DJIMOTOR_HPP_
#define _DJIMOTOR_HPP_

#include "../User/LowLayer/HAL_/can/can_driver.hpp"
#include "../User/MidLayer/Managers/state_manager/state.hpp"

#define MY_PI 3.1415926535897932384626433832795

namespace motor
{
    struct AngleCalculator 
    {
        float maxAngle;           // 最大角度值
        float accumulatedAngle;   // 累计角度
        float lastAngleRaw;       // 上一次原始角度值
        bool initialized;         // 初始化标志
        
        AngleCalculator(float maxAngleValue) : 
            maxAngle(maxAngleValue), 
            accumulatedAngle(0.0f), 
            lastAngleRaw(0.0f), 
            initialized(false) {}
        
        // 更新累计角度
        float update(float currentRawAngle) 
        {
            if (!initialized) 
            {
                lastAngleRaw = currentRawAngle;
                accumulatedAngle = currentRawAngle;
                initialized = true;
                return accumulatedAngle;
            }
            
            // 处理角度翻转情况
            float delta = currentRawAngle - lastAngleRaw;
            const float halfMax = maxAngle / 2.0f;
            
            if (delta > halfMax) 
            {
                // 向下翻转
                accumulatedAngle += (-maxAngle + delta);
            } 
            else if (delta < -halfMax) 
            {
                // 向上翻转
                accumulatedAngle += (maxAngle + delta);
            }
            else 
            {
                // 正常变化
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
                // 查找是否存在对应的角度计算器
                for (auto& calc : angleCalculators) 
                {
                    if (calc.maxAngle == maxAngleValue) 
                    {
                        return calc.update((float)angle);
                    }
                }
                
                // 如果不存在，则创建新的角度计算器
                angleCalculators.emplace_back(maxAngleValue);
                return angleCalculators.back().update((float)angle);
            }

            float totalAngle;               //累计角度
            float MotorSet;                 //电机设置值
        
        private:
            uint32_t recv_id_;              //接收id
            int16_t ref_angle;              //转子机械角度  0-8191
            int16_t ref_speed;              //转子速度      rpm
            int16_t ref_torqueCurrent;      //实际扭矩电流
            uint8_t ref_temperate;          //电机温度      摄氏度

            float angle;                    //转子机械角度,float              
            float speed;                    //转子速度,float        
            float torquecurrent;            //实际扭矩电流,float
            float torque;                   //电机扭矩

            float angle_8191;               //转子机械角度  0-8191
            float angle_360;                //转子机械角度  0-360
            float speed_rad;                //转子速度      rad/s

            //提供两个接口，这个是最大角度的值，前面的是结构体，里面有一些关于累加角度的变量和函数
            AngleCalculator angle8191Calc{8191.0f};
            AngleCalculator angle360Calc {360.0f };
            std::vector<AngleCalculator> angleCalculators;
    };

    class DJIMotorController 
    { 
    public:
        explicit DJIMotorController(CanDriver::CanHal& can) : can_(can) {}
    
        // 更清晰的API命名
        bool Send0x200(int16_t m1, int16_t m2, int16_t m3, int16_t m4) //值都放入这条函数
        {
            return SendMotorData(0x200, m1, m2, m3, m4);
        }
        
        bool Send0x1FF(int16_t m1, int16_t m2, int16_t m3, int16_t m4) //值都放入这条函数
        {
            return SendMotorData(0x1FF, m1, m2, m3, m4);
        }

        bool Send0x1FE(int16_t m1, int16_t m2, int16_t m3, int16_t m4) //值都放入这条函数
        {
            return SendMotorData(0x1FE, m1, m2, m3, m4);
        }

    private:
        bool SendMotorData(uint16_t cmdId, int16_t m1, int16_t m2, int16_t m3, int16_t m4); //发送帧处理函数
        CanDriver::CanHal& can_;
        
        static void PackInt16(uint8_t* dst, int16_t val) //数据处理函数
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
