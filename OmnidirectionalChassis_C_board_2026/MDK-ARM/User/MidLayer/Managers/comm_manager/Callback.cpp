#include "Callback.hpp"

uint8_t send_str2[sizeof(float) * 8]; // ����8��float�ռ䣨32�ֽڣ�
void vofa_send(float x1, float x2, float x3, float x4, float x5, float x6) 
{
    const uint8_t sendSize = sizeof(float); // ��������ռ4�ֽ�

    // ��6����������д�뻺������С��ģʽ��
    *((float*)&send_str2[sendSize * 0]) = x1;
    *((float*)&send_str2[sendSize * 1]) = x2;
    *((float*)&send_str2[sendSize * 2]) = x3;
    *((float*)&send_str2[sendSize * 3]) = x4;
    *((float*)&send_str2[sendSize * 4]) = x5;
    *((float*)&send_str2[sendSize * 5]) = x6;

    // д��֡β��Э��Ҫ�� 0x00 0x00 0x80 0x7F��
    *((uint32_t*)&send_str2[sizeof(float) * 6]) = 0x7F800000; // С�˴洢Ϊ 00 00 80 7F

    // ͨ��DMA��������֡��6���� + 1֡β = 7��float����28�ֽڣ�
    HAL_UART_Transmit_DMA(&huart6, send_str2, sizeof(float) * 7);
}

