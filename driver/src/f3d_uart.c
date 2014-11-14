/* f3d_uart.c --- 
 * 
 * Filename: f3d_uart.c
 * Part of: Lab5
 * Description: defines pin initialization functions and char I/O functions
 * Author: Zhongren Shao (shaoz), Erin Leonhard (eeleonha) 
 * Created: 9/26/2014
 * Last-Updated: 9/26/2014
 *           By: Authors
 * 
 */

#include <stm32f30x.h>
#include <f3d_led.h>

queue_t txbuf, rxbuf;

void f3d_uart_init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC,&GPIO_InitStructure);

  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC , &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOC,4,GPIO_AF_7);
  GPIO_PinAFConfig(GPIOC,5,GPIO_AF_7);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1 ,&USART_InitStructure);
  USART_Cmd(USART1 , ENABLE);

  // Initialize the rx and tx queues
  init_queue(&rxbuf);
  init_queue(&txbuf);

  // Setup the NVIC priority and subpriority
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x08;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x08;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  // Enable the RX interrupt 
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
}

void USART1_IRQHandler(void) {
  int ch; 

  if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)) {
    ch = USART_ReceiveData(USART1);
    while (!enqueue(&rxbuf,ch));
  }
  if (USART_GetFlagStatus(USART1,USART_FLAG_TXE)) {
    ch = dequeue(&txbuf);
    if (ch) {
      USART_SendData(USART1,ch);
    }
    else {
      USART_ITConfig(USART1,USART_IT_TXE,DISABLE); 
    }
  }
}

// putschar from UART to serialT
int putchar(int c) {
  // doesn't send data until USART is available
  while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == (uint16_t)RESET);

  // send data to UART
  // USART_SendData(USART1, c);
  enqueue(&txbuf, c);
  return 0;
} 

// getschar from serialT to UART
int getchar(void) {
  // ditto as putchar
  if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != (uint16_t)RESET);

  // recv data from UART
  char c = dequeue(&rxbuf);
  //USART_ReceiveData(USART1);
  
  return c;
}

// putstring from UART to serialT
void putstring(char *s) {
  int i = 0;
  while(s[i] != '\0') {
    putchar(s[i++]);
  }
}

void flush_uart(void) {
  USART_ITConfig(USART1,USART_IT_TXE,ENABLE); 
}

/* f3d_uart.c ends here */
