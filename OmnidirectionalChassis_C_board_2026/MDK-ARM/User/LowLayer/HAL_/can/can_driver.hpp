#ifndef _CAN_DRIVER_HPP_
#define _CAN_DRIVER_HPP_

#include <can.h>
#include <functional>
#include <memory>

namespace CanDriver
{
    struct CanFrame 
    {
        uint32_t id{};
        uint8_t  dlc{};
        bool     isExtended{};
        bool     isRemote{};
        uint8_t  data[8]{};
    };

    class CanHal
    {
        public:
            virtual ~CanHal() = default;
            CAN_HandleTypeDef hcan_;
            void ConfigureFilter();
            void IrqRxFifo0();
            using RxCallback = std::function<void(const CanFrame&)>; //������һ���ձ��������������һ����������ʵ�������ɴ���һ�����ͽ�RxCallback��
            virtual bool Send(const CanFrame& frame) = 0;
            void SetRxCallback(RxCallback callback) //���α�����RxCallback callback�Լ� RmMotorRxData ������������ RxCallback ��ԭ����RmMotorRxData�����Ĵ����Լ��������ͺ�void(const CanFrame&)һ��
            {
                rxCallback_ = callback; //��callback�����ձ���������RxCallback���͵ı���rxCallback_������callback��ֵ��
            }

        private:
            RxCallback rxCallback_; // ���������˵�Ļص��������� ��һ�������������ձ�������������RxCallback������һ��������rxCallback_��
    };

    class STM32CANHal : public CanHal
    {
        public:
            bool Send(const CanFrame& frame) override 
            {
                CAN_TxHeaderTypeDef txHeader;
                txHeader.StdId = frame.id;
                txHeader.ExtId = frame.id; 
                txHeader.IDE = frame.isExtended ? CAN_ID_EXT : CAN_ID_STD;
                txHeader.RTR = frame.isRemote ? CAN_RTR_REMOTE : CAN_RTR_DATA;
                txHeader.DLC = frame.dlc;
                txHeader.TransmitGlobalTime = DISABLE;

                uint32_t mailbox;
                return HAL_CAN_AddTxMessage(&hcan_, &txHeader, const_cast<uint8_t*>(frame.data), &mailbox) == HAL_OK;
            }


    };

    std::unique_ptr<CanDriver::CanHal> CreateCANHal(uint32_t index);
}

void CAN1_RxFifo0Callback(void);
void CAN2_RxFifo0Callback(void);

#endif
