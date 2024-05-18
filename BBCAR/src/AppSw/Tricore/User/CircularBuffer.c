/*
 * CircularBuffer.c
 *
 *  Created on: 2024年4月25日
 *      Author: dduu
 */
#include <CircularBuffer.h>

// 初始化环形缓冲区
void CircularBuffer_Init(CircularBuffer* cb) {
    cb->head = 0;
    cb->tail = 0;
}


// 判断环形缓冲区是否已满
int CircularBuffer_IsFull(const CircularBuffer* cb) {
    if((cb->head + 1) % BUFFER_SIZE == cb->tail){
        return 1;
    }else{
        return 0;
    }
}

// 判断环形缓冲区是否为空
int CircularBuffer_IsEmpty(const CircularBuffer* cb) {
    if(cb->head == cb->tail){
        return 1;
    }else{
        return 0;
    }
}

// 向环形缓冲区写入数据
int CircularBuffer_Write(CircularBuffer* cb, uint8 data) {
  if (CircularBuffer_IsFull(cb)) {  // 缓冲区已满，无法写入
    return 0;
  }

  cb->buffer[cb->head] = data;
  cb->head = (cb->head + 1) % BUFFER_SIZE;
  return 1;
}

// 从环形缓冲区读取数据
int CircularBuffer_Read(CircularBuffer* cb, uint8* data) {
  if (CircularBuffer_IsEmpty(cb)) {  // 缓冲区为空，无数据可读取
    return 0;
  }

  *data = cb->buffer[cb->tail];
  cb->tail = (cb->tail + 1) % BUFFER_SIZE;
  return 1;
}

// 获取环形缓冲区剩余大小
uint16 CircularBuffer_GetRemainingSize(const CircularBuffer* cb) {
  if (cb->head >= cb->tail) {
    return BUFFER_SIZE - (cb->head - cb->tail);
  } else {
    return cb->tail - cb->head - 1;
  }
}

// 获取环形缓冲区已写入大小
uint16 CircularBuffer_GetWrittenSize(const CircularBuffer* cb) {
  if (cb->head >= cb->tail) {
    return cb->head - cb->tail;
  } else {
    return BUFFER_SIZE - (cb->tail - cb->head - 1);
  }
}

// 从环形缓冲区读取指定长度的数据
int CircularBuffer_ReadData(CircularBuffer* cb, uint8* data, uint16 length) {
  if (CircularBuffer_GetWrittenSize(cb) < length) {
    return 0;  // 缓冲区中的数据不足
  }

  for (uint16 i = 0; i < length; ++i) {
    if (!CircularBuffer_Read(cb, &data[i])) {
      return 0;  // 读取数据出错
    }
  }

  return 1;
}

// 向环形缓冲区写入指定长度的数据
int CircularBuffer_WriteData(CircularBuffer* cb, const uint8* data, uint16 length) {
  if (CircularBuffer_GetRemainingSize(cb) < length) {
    return 0;  // 缓冲区剩余空间不足
  }

  for (uint16 i = 0; i < length; ++i) {
    if (!CircularBuffer_Write(cb, data[i])) {
      return 0;  // 写入数据出错
    }
  }

  return 1;
}


// 示例：STM32串口接收中断处理函数
//void USART_Receive_IRQHandler(void) {
//  uint8 data = USART_ReceiveData(USART1);  // 获取接收到的数据
//  if (!CircularBuffer_Write(&rxBuffer, data)) {
//    // 缓冲区已满，处理错误
//  }
//}






