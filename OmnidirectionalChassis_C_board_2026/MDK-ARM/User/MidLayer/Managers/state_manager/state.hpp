#ifndef _STATE_HPP_
#define _STATE_HPP_

#include "main.h"

// ǰ������
namespace motor {
    class DJImotor;
}

namespace Clicker {
    class DR16;
}

// �ⲿ��������
extern motor::DJImotor M3508_201;
extern motor::DJImotor M3508_202;
extern motor::DJImotor M3508_203;
extern motor::DJImotor M3508_204;

extern Clicker::DR16 dr16;


namespace State
{
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
			void updateState();  // ������ʵ����.cpp�ļ���
			model_state getCurrentState() 
			{
				return current_state;
			}

        private:
            model_state current_state = STOP;
        
    };
}

void MotorState();
void RemoteState();

#endif
