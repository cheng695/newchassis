#include "state.hpp"
#include "../User/LowLayer/Equipment/motor/djimotor.hpp"
#include "../User/LowLayer/Equipment/remote/dr16.hpp"

void MotorState()
{
    M3508_201.getlastTime();
    M3508_201.checkTime(200);
    if(!M3508_201.isOnline)
    {

    }

    M3508_202.getlastTime();
    M3508_202.checkTime(200);
    if(!M3508_202.isOnline)
    {
        
    }

    M3508_203.getlastTime();
    M3508_203.checkTime(200);
    if(!M3508_203.isOnline)
    {
        
    }

    M3508_203.getlastTime();
    M3508_203.checkTime(200);
    if(!M3508_203.isOnline)
    {
        
    }
}

void RemoteState()
{
    dr16.getlastTime();
    dr16.checkTime(200);
    if(!dr16.isOnline)
    {

    }
}

void 