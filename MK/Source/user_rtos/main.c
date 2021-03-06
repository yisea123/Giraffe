/*
    
*/

/******************************************************************************
 
 */


#include "main.h"
#include "fsmc_fpga.h"
#include "user_tasks.h"
#include "console.h"

/*Mutex*/
SemaphoreHandle_t xMutexFSMC= 				NULL;
SemaphoreHandle_t xMutexUSART_CONSOLE=NULL;
SemaphoreHandle_t xMutexSPI_WIZ=			NULL;
//SemaphoreHandle_t xMutexUSART=				NULL;
/*Semaphore*/
SemaphoreHandle_t xSemaphoreEXTI= 		NULL;
SemaphoreHandle_t xSemaphoreFSMCDMA= 	NULL;
SemaphoreHandle_t xSemaphoreSPIDMA= 	NULL;
SemaphoreHandle_t xSemaphoreCONSOLE= 	NULL;

/*Queue*/

extern char bufer_cons_out[SIZE_CONS_OUT];

/***********************************************/
void vApplicationTickHook( void )
{
	
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */

	for( ;; );
}
/*-----------------------------------------------------------*/



/*******************************************************************/
void vFreeRTOSInitAll()
{
		CONSOLE_USART_INIT();
		console_send("\r\n\r\nDevice_start\r");

		LED_INIT();
	
	
	
		
		#ifdef EEPROM
			I2C_EE_INIT();
			if (ReadConfig()==TRUE)	console_send("\nEEPROM start\r");
			else console_send("\nEEPROM is not connect \r\n parameters is enabled by default\r");
		#else
				SettingsDefault();
				CheckAndWriteVersion();
			  PrintVersion(bufer_cons_out);
				console_send(bufer_cons_out);
				
			
		#endif

		#ifdef FSMC
			FSMC_FPGA_Init();
			if (FSMC_FPGA_Detect()==TRUE)	console_send("\nFPGA is connect\r");
			else console_send("\nFPGA is not detect, check the connection \r");
		#endif

		#ifdef WIZNET
			WIZ_GPIO_Install();
			WIZ_Config();
			console_send("\nWIZNET start\r");
		#endif
	
	
}
 
 void TEST_INIT(void)
{
	
		GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(LED_GPIO_RCC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
 
    GPIO_StructInit(&GPIO_InitStructure);
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin = LED1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(LED_GPIO, &GPIO_InitStructure);
	  GPIO_SetBits(LED_GPIO,LED1);
	
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(LED_GPIO, &GPIO_InitStructure);
	  
		GPIO_SetBits(LED_GPIO,GPIO_Pin_12);
		GPIO_SetBits(LED_GPIO,GPIO_Pin_14);
		GPIO_SetBits(LED_GPIO,GPIO_Pin_15);
	
	
	
		  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  
	
}
 
void vLedTask2(void *pvParameters)
{
	bool led=true;
	
    while(1)
    {
			if (led)
			{
				GPIO_ResetBits(LED_GPIO,GPIO_Pin_15);
				led=0;
			}	
			else
			{
				GPIO_SetBits(LED_GPIO,GPIO_Pin_15);
				led=1;
			}
			vTaskDelay(5000);
		}
    vTaskDelete(NULL);	
}

 void vLedTask1 (void *pvParameters)
{
	bool led=true;
	
    while(1)
    {
			ChangeLED();
			if ( GPIO_ReadInputDataBit_BOOL(GPIOA,GPIO_Pin_0))
			{
				xTaskCreate(vLedTask2,(signed char *)"LedTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1 , NULL);	

					if (led)
					{
						GPIO_ResetBits(LED_GPIO,GPIO_Pin_15);
						led=false;
					}	
					else 
					{
						GPIO_SetBits(LED_GPIO,GPIO_Pin_15);
						led=true;
					}
			}
			
			vTaskDelay(PERIOD_LED_TASK);
		}
    vTaskDelete(NULL);	
}
/*******************************************************************/
int main(void)

{
	
	xMutexUSART_CONSOLE = xSemaphoreCreateMutex();
	vSemaphoreCreateBinary(xSemaphoreCONSOLE);
	
	#ifdef FSMC
		vSemaphoreCreateBinary(xSemaphoreFSMCDMA);
		vSemaphoreCreateBinary(xSemaphoreEXTI);
		xMutexFSMC = xSemaphoreCreateMutex();
	#endif
	

	#ifdef WIZNET
		xMutexSPI_WIZ =  xSemaphoreCreateMutex();
		vSemaphoreCreateBinary(xSemaphoreSPIDMA);
	#endif
		
  vFreeRTOSInitAll();
	//	TEST_INIT();
	
//	xTaskCreate(vLedTask1,(signed char *)"LedTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1 , NULL);	

	
	if ((xMutexFSMC != NULL)&&(xSemaphoreEXTI !=NULL)&&(xSemaphoreFSMCDMA!=NULL)&&(xSemaphoreCONSOLE!=NULL)&&(xMutexUSART_CONSOLE!=NULL))
		{
			#ifdef FSMC	
				xTaskCreate(ProcessingIntFPGA,(signed char*)"ProcessingIntFPGA", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+4 , NULL);
				xTaskCreate(StartCalcBuferFPGA,(signed char*)"StartCalcBufer", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2 , NULL);
			#endif
			
			#ifdef WIZNET
				xTaskCreate(TCP_IPConnect,(signed char*)"TCP_IPConnect", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2 , NULL);
			#endif
			
			xTaskCreate(vLedTask,(signed char *)"LedTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1 , NULL);
//			xTaskCreate(ConsoleExchange,(signed char *)"ConsoleExchange", configMINIMAL_STACK_SIZE*10, NULL, tskIDLE_PRIORITY+1 , NULL);
			console_send("\n>");	

			vTaskStartScheduler();
		}

	
	vTaskStartScheduler();
	for( ;; );
}

