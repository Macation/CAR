/*
 * CircularBuffer.h
 *
 *  Created on: 2024Äê4ÔÂ25ÈÕ
 *      Author: dduu
 */

#ifndef SRC_APPSW_TRICORE_USER_CIRCULARBUFFER_H_
#define SRC_APPSW_TRICORE_USER_CIRCULARBUFFER_H_

#include <stdio.h>
#include <stdlib.h>
#include <Platform_Types.h>

#define BUFFER_SIZE 500

typedef struct {
  uint8 buffer[BUFFER_SIZE];
  uint16 head;
  uint16 tail;
} CircularBuffer;

void CircularBuffer_Init(CircularBuffer* cb);
int CircularBuffer_IsFull(const CircularBuffer* cb);
int CircularBuffer_IsEmpty(const CircularBuffer* cb);
int CircularBuffer_Write(CircularBuffer* cb, uint8 data);
int CircularBuffer_Read(CircularBuffer* cb, uint8* data);
uint16 CircularBuffer_GetRemainingSize(const CircularBuffer* cb);
uint16 CircularBuffer_GetWrittenSize(const CircularBuffer* cb);
int CircularBuffer_ReadData(CircularBuffer* cb, uint8* data, uint16 length);
int CircularBuffer_WriteData(CircularBuffer* cb, const uint8* data, uint16 length);


#endif /* SRC_APPSW_TRICORE_USER_CIRCULARBUFFER_H_ */
