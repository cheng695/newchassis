#ifndef _STATE_HPP_
#define _STATE_HPP_

#include "main.h"

// 前向声明，避免包含djimotor.hpp
namespace motor 
{
    class DJImotor;
}

namespace Clicker
{
    class DR16;
}

extern motor::DJImotor M3508_201;
extern motor::DJImotor M3508_202;
extern motor::DJImotor M3508_203;
extern motor::DJImotor M3508_204;

extern Clicker::DR16 dr16;

namespace State
{
    class monitoring
    {
        public:
            int lastUpdateTime;
            int currentTime;
            bool isOnline;
            void getlastTime()
            {
                currentTime = HAL_GetTick();
            }

            void getTime()
            {
                lastUpdateTime = HAL_GetTick();
            }
            
            bool checkTime(int time)
            {
                if (currentTime - lastUpdateTime > time)
                {
                    isOnline = false;
                }
                return isOnline;
            }
    };

    class model
    {
        enum model_state
        {
            STOP,      //2,2
            FOLLOW,    //2,3
            NOTFOLLOW, //3,2
            SHOT,      //2,1
            VISION,    //1,2
            KEYBOARD,  //1,1
        };
        public:
               void updateState() 
               {
                    // 根据遥控器s1和s2的值更新状态
                    if (dr16.rc.s1 == 2 && dr16.rc.s2 == 2) 
                    {
                        current_state = STOP;
                    }
                    else if (dr16.rc.s1 == 2 && dr16.rc.s2 == 3) 
                    {
                        current_state = FOLLOW;
                    }
                    else if (dr16.rc.s1 == 3 && dr16.rc.s2 == 2) 
                    {
                        current_state = NOTFOLLOW;
                    }
                    else if(dr16.rc.s1 == 2, dr16.rc.s2 == 1)
                    {
                        current_state = SHOT;
                    }
                    else if(dr16.rc.s1 == 1, dr16.rc.s2 == 2)
                    {
                        current_state = VISION;
                    }
                    else if(dr16.rc.s1 == 1, dr16.rc.s2 == 1)
                    {
                        current_state = KEYBOARD;
                    }
                }
                
                model_state getCurrentState() 
                {
                    return current_state;
                }

        private:
            model_state current_state = STOP;
        
    };
}

void MotorState();

#endif
