
/* Standard includes. */

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "ff.h"

#include <system.h>
#include <stdio.h>

/* Hardware Includes */
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

#include <i2c.h>
#include <spi.h>
#include <uart.h>
#include <clock.h>
#include <command.h>
#include <string.h>
#include "ff.h"
#include "diskio.h"

static int Test1();

/////////////////////////////////////////////////////////////////////




/* Application includes */

/*Init system config*/

#define BURN_DELAY          5 /*Seconds until antenna burn*/


static void initTask(void * params);
static int ReadSDCard();

static void lowLevelHardwareInit()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
}

static void initializeBackupRegisters()
{  
    int i;
    
    bitSet(PWR->CR, PWR_CR_DBP);   
    for (i = 0; i < 2; i++) {} /* Delay for bus write */ 
   
    /* RTC_BKP0R (Status Flags)
      Bit 0: Antenna Status (0 undeployed, 1 deployed)
    */
    RTC->BKP0R = 0x00000000;
    
    bitClear(PWR->CR, PWR_CR_DBP);
}

int main( void )
{
     lowLevelHardwareInit();

    SysTick_Config(3000);
        //Test1();
       disk_initialize(0);
       ReadSDCard(); //debug remove

       return (1);

    xTaskCreate(initTask, NULL, systemDEFAULT_STACK_SIZE, NULL, systemPRIORITY_INIT, NULL);
    vTaskStartScheduler();
    
    
   /* Will only reach here if there is insufficient heap available to start
    the scheduler. */
    return 0;
}

void initTask(void * params)
{
    /* Test Heartbeat Initialization */
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /*  End test heartbeat initialization */
    
    /* Begin actual init */
    int i;
    
    I2C_Initialize();
    //SPI_Initialize();
    
    vUartStartTask();
    vConsolePrintf("\r\n\r\n==================== BOOT ====================\r\n");
    vConsolePrintf("Init task started\r\n");

    vConsolePrintf("Checking RTC initialization status...\r\n");
    if (!(RTC->ISR & RTC_ISR_INITS)) { /*This is first boot*/
        vConsolePrintf("RTC Not Initialized. Performing first boot routine.\r\n");
        vConsolePrintf("Initializing RTC...");
        startRTC();
        vConsolePrintf("Done.\r\n");
        vConsolePrintf("Delaying for RTC synchronization...");
        vTaskDelay(2500);
        vConsolePrintf("Done\r\n");

        vConsolePrintf("Initializing Backup Register...");
        initializeBackupRegisters();
        vConsolePrintf("Done\r\n");
        
        vConsolePrintf("First boot routine complete.\r\n");
    }
    else
    {
        vConsolePrintf("RTC already initialized.\r\n");
    }

    char timeBuffer[15];
    sprintf(timeBuffer, "Mission Time: %ld\r\n", getMissionTime());
    vConsolePrintf(timeBuffer);
    vConsolePrintf("Waiting for deployment delay...\r\n");

    while(getMissionTime() <= BURN_DELAY) {
        vTaskDelay(500);
    }
    
    vConsolePrintf("Deployment delay reached - checking status...\r\n");
    if (!(RTC->BKP0R & ANTENNA_STATUS)) {
        vConsolePrintf("Deploying Antennas...");
//TODO: Call antenna deployment function
        vConsolePrintf("Done\r\n");
        
        vConsolePrintf("Setting antenna status flag...");
        bitSet(PWR->CR, PWR_CR_DBP);     
        for (i = 0; i < 2; i++) {} /* Delay for bus write */ 
        bitSet(RTC->BKP0R, ANTENNA_STATUS);
        bitSet(PWR->CR, PWR_CR_DBP);        
        vConsolePrintf("Done\r\n");
    }
    else
    {
      vConsolePrintf("Antennas already deployed!\r\n");
    }

    vConsolePrintf("Starting command handling on console...");
    xStartUart1CommandHandling();
    vConsolePrintf("Done\r\n");

//    vConsolePrintf("Starting storage driver task\r\n");
//TODO: Port    startStorageDriverTask();
    
    vConsolePrintf("Init finished!\r\n");
    vConsolePrintf("Testing SD Write!\r\n");
    


#if 1
    uint8_t i2cbuffer[1000];
    uint8_t spitxbuffer[100];
    uint8_t spirxbuffer[100];
#endif
    
    vConsolePrintf("Testing SPI transfer...");
    //test pattern to send to spi port via DMA
         spitxbuffer[0] =0x01;
         spitxbuffer[1] =0x02;
         spitxbuffer[2] =0x04;
         spitxbuffer[3] =0x06;
         spitxbuffer[4] =0x08;
         spitxbuffer[5] =0x01;

      //SPI1_Transfer(spitxbuffer, spirxbuffer,6, portMAX_DELAY);
     vConsolePrintf("Spi Transfer Done\r\n");


    for(;;)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
        vTaskDelay(500); //
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        vTaskDelay(500);

        vConsolePrintf("Testing SPI transfer...");
        SPI1_Transfer(spitxbuffer, spirxbuffer,5, portMAX_DELAY);
        vConsolePrintf("Spi Transfer Done\r\n");

        continue; //debug. remove later

#if 1
        /* I2C test */
        
        vConsolePrintf("Debug1!\r\n");

        bzero(i2cbuffer, 100);
        I2CSYS_TakeMutex(portMAX_DELAY);
        i2cbuffer[0] = 10;
        I2CSYS_Write(i2cbuffer, 0x3C, 1, portMAX_DELAY);
        I2CSYS_Read(i2cbuffer, 0x3C, 3, portMAX_DELAY);
        
        vConsolePrintf("I2C Test Bytes: %x %x %x\r\n", i2cbuffer[0], i2cbuffer[1], i2cbuffer[2]);
        
        i2cbuffer[0] = 0x00;
        I2CSYS_Write(i2cbuffer, 0x3C, 1, portMAX_DELAY);
        I2CSYS_Read(i2cbuffer, 0x3C, 3, portMAX_DELAY);
        vConsolePrintf("I2C Pre-Write Bytes: %x %x %x\r\n", i2cbuffer[0], i2cbuffer[1], i2cbuffer[2]);
        
        i2cbuffer[0] = 0x02;
        I2CSYS_Write(i2cbuffer, 0x3C, 1, portMAX_DELAY);
        
        i2cbuffer[1] = 0x03;
        i2cbuffer[2] = 0x04;
        i2cbuffer[3] = 0x05;
        i2cbuffer[4] = 0x06;
        I2CSYS_Write(i2cbuffer, 0x3C, 5, portMAX_DELAY);
        
        i2cbuffer[0] = 0x00;
        I2CSYS_Write(i2cbuffer, 0x3C, 1, portMAX_DELAY);
        I2CSYS_Read(i2cbuffer, 0x3C, 3, portMAX_DELAY);
        
        
        vConsolePrintf("I2C Readback Bytes: %x %x %x\r\n", i2cbuffer[0], i2cbuffer[1], i2cbuffer[2]); 
        /*  Long read test 
        vConsolePrintf("Beginning long read. (100 1KB reads).  Should take 8 seconds\r\n");
        for(i = 0; i < 100; i++)
        {
            I2CSYS_Write(i2cbuffer, 0x3C, 1, portMAX_DELAY);
            I2CSYS_Read(i2cbuffer, 0x3C, 1000, portMAX_DELAY);
        }
        vConsolePrintf("Long read done\r\n");*/
        
        I2CSYS_ReleaseMutex();
        
        vConsolePrintf("Testing SPI transfer...");
        SPI1_TakeMutex(portMAX_DELAY);
        SPI1_Transfer(spitxbuffer, spirxbuffer, 100, portMAX_DELAY);
        SPI1_ReleaseMutex();
        vConsolePrintf("Done\r\n"); //
#endif
    }
}

int ReadSDCard()
{
	UINT bw;

	/** Read a text file and display it */

	FATFS FatFs;   /* Work area (file system object) for logical drive */
    FIL fil,fil1;       /* File object */
    //char line[82]; /* Line buffer */
    FRESULT fr;    /* FatFs return code */
    //FIL fsrc;      // File objects
    BYTE buffer[512];    /* was 4096 File copy buffer */
    UINT br;


	     /* Register work area to the default drive **/
	    f_mount(&FatFs, "", 0);
	    /* Open a text file */

	    if (f_open(&fil1, "file.txt", FA_WRITE | FA_OPEN_ALWAYS) == FR_OK)
	    {	// Open existing or create new file
	    		if (f_lseek(&fil1, f_size(&fil1)) == FR_OK)
	    			{
	    				char *text2 = "This is a new line, appended to existing file!\r\n";
	    				f_write(&fil1, text2, strlen(text2), &bw); // Write data to the file
	    				if (bw == strlen(text2))
	    				{ //we wrote the entire string
	    					printf("Appended:%d\n", (int)f_size(&fil1));
	    				}


	    			}
	    	f_close(&fil1);// Close the file
	    }

	    char buff[50];
	    strcpy(buff,"file.txt");
	    fr = f_open(&fil, "Bourbour.txt", FA_READ);
	    //fr = f_open(&fil,buff, FA_READ);
	    if (fr) return (int)fr;

	    for (;;)
	    		{
	            	fr = f_read(&fil, buffer, sizeof buffer, &br);  /* Read a chunk of source file */
	            	//fr = f_read(&fsrc, buffer,1, &br);  /* Read a chunk of source file */
	            	if (fr || br == 0)
	            		break; /* error or eof */
	            	 printf(buffer);
	           	 }
	    /* Close the file */
	    f_close(&fil);

	    return 0;
}

void vApplicationStackOverflowHook()
{
  for(;;) {}
}

void vApplicationMallocFailedHook()
{
  for(;;) {}
}


int Test1 ()
{
    int rc;
    DWORD buff[512];  /* 2048 byte working buffer  **/

      /****  Check function/compatibility of the physical drive #0 **/
    rc = test_diskio(0, 1, buff, sizeof buff);
    if (rc) {
    	 vConsolePrintf("Sorry the function/compatibility test failed.\nFatFs will not work on this disk driver.\n");
    } else {
    	vConsolePrintf("Congratulations! The disk I/O layer works well.\n");
    }

    return rc;
}

