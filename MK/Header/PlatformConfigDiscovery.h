

#include "stm32f4xx.h"

#include "Types.h"

typedef  uint8_t fpga_size_byte;
typedef vu8 bit_bus ;
typedef uint8_t bit_data;



/* WIZNET SPI Settings*/
#define WIZ_SPI_PORT_RCC				RCC_APB2Periph_SPI1
#define WIZ_PORT_SETTINGS				GPIOC
#define WIZ_RESET		        		GPIO_Pin_2	// out
#define WIZ_PWDN		        		GPIO_Pin_1	// out

#define WIZ_PORT_INT					  GPIOA
#define WIZ_INT			        		GPIO_Pin_2  // in

#define WIZ_SPI 								SPI1
#define WIZ_SPI_PORT 						GPIOA
#define WIZ_SCS			  					GPIO_Pin_3	// out
#define WIZ_SCLK								GPIO_Pin_5	// out
#define WIZ_MISO								GPIO_Pin_6	// in
#define WIZ_MOSI								GPIO_Pin_7	// out

#define GPIO_AF_SPI 						GPIO_AF_SPI1
#define WIZ_SCLK_AF 						GPIO_PinSource5
#define WIZ_MISO_AF 						GPIO_PinSource6
#define WIZ_MOSI_AF 						GPIO_PinSource7

/*USART settings*/	

#define USART									USART1
#define USART_CLK             RCC_APB2Periph_USART1
#define USART_PORT 						RCC_AHB1Periph_GPIOA 
#define USART_GPIO						GPIOA
#define USART_GPIO_RX					GPIO_Pin_10
#define USART_GPIO_TX					GPIO_Pin_9	
#define USART_AF           		GPIO_AF_USART1
#define USART_TX_SOURCE       GPIO_PinSource9
#define USART_RX_SOURCE       GPIO_PinSource10
#define USART_IRQ							USART1_IRQn

/*LED GPIO Settings*/

#define LED_GPIO_RCC 						RCC_AHB1Periph_GPIOD
#define LED_GPIO 								GPIOD
#define LED1										GPIO_Pin_13	// out

/*Interrut from FPGA*/

#define EXT_INT_FPGA_Channel 		EXTI9_5_IRQn
#define EXT_LINE_INT_FPGA 			EXTI_Line6

/*FSMC width bus*/

#define FSMC_8bit
//#define FSMC_16bit

/*Settings for DMA FSMC*/

#define DMA_FSMC_STREAM               DMA2_Stream0
#define DMA_FSMC_CHANNEL              DMA_Channel_0
#define DMA_FSMC_STREAM_CLOCK         RCC_AHB1Periph_DMA2   
#define DMA_FSMC_STREAM_IRQ           DMA2_Stream0_IRQn
#define DMA_FSMC_IT_TC	              DMA_IT_TC
#define DMA_FSMC_IT_TCIF_CHECK 				DMA_IT_TCIF0

/*Settings for I2C*/

#define I2C                          	I2C1
#define I2C_RCC 											RCC_APB1Periph_I2C1
#define I2C_RCC_PORT 									RCC_AHB1Periph_GPIOB
#define GPIO_Pin_SDA 									GPIO_Pin_7
#define GPIO_Pin_SCL 									GPIO_Pin_6
#define I2C_PORT											GPIOB
#define I2C_SDA_SOURCE								GPIO_PinSource6
#define I2C_SCL_SOURCE								GPIO_PinSource7
#define I2C_GPIO_AF										GPIO_AF_I2C1 

#define EEPROM_ADDRESS								0xFF

