#include <vector>
#ifndef _ODOMETRY_HPP_
#define _ODOMETRY_HPP_ 

namespace odometry
{
    class angle_odometry
    {
        public:
            angle_odometry(float MaxAngle, float Resolution) : maxAngle(MaxAngle), Resolution(Resolution)
            {
                accumulatedAngle = 0.0f;
                lastAngleRaw = 0.0f;
                Resolution_coefficient = MaxAngle / Resolution;
                initialized = false;
            }

            float update(float currentRawAngle)
            {
                currentRawAngle *= Resolution_coefficient;

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

        private:
            float maxAngle;           // ���Ƕ�ֵ
            float Resolution;         // �������ֱ���
            float accumulatedAngle;   // �ۼƽǶ�
            float lastAngleRaw;       // ��һ��ԭʼ�Ƕ�ֵ
            float Resolution_coefficient; //�ֱ���ϵ��
            bool initialized;         // ��ʼ����־
    };
}

#endif
