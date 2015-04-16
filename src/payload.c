
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "system.h"
#include "uart.h"
#include "wire.h"
#include "payload.h"
#include "myprintf.h"
#include "ff.h"
#include "diskio.h"
#include "payload.h"
#include "string.h"

#define payloadRADIOMETERS 3
#define payloadCHANNELS 10
#define payloadREGISTER 0x00

#define payloadBUS_PROTOTYPE  wireBUS1
#define payloadADDR_PROTOTYPE ( 0xA0 )

#define payloadBUS1	wireBUS2
#define payloadBUS2	wireBUS3
#define payloadBUS3	wireBUS4

#define payloadENABLE1_TRIS _TRISB4
#define payloadENABLE2_TRIS _TRISB5
#define payloadENABLE3_TRIS _TRISE9

#define payloadADDR ( 0xA0 )

#define payloadBLOCK_TIME ( ( portTickType ) 0xffff )


static void vPayloadPrototypePollData();
static void vPayloadPollData();
static void vPayloadTask( void *pvParameters );
static void vPayloadPrintData( char pcData[][payloadCHANNELS] );

static int vPayloadGetImageOnSDCard();
static int vPayloadDeleteImageOnSDCard(char*);
static int vPayloadWriteToSDCard(char* file_name,char* data_string);
static char* vPayloadGetImageFileName(char*);

void vPayloadStartTask()
{
    xTaskCreate( vPayloadTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_PAYLOAD, NULL );
}

static void vPayloadPrintData( char pcData[][payloadCHANNELS] )
{
    unsigned char i, j;
    char out[20];

    vConsolePrint( "Radi\tChan\tValue\r\n" );

    for( i=0; i<payloadRADIOMETERS; ++i )
    {
        for( j=0; j<payloadCHANNELS; ++j )
        {
            vTaskDelay(10);
            //mysnprintf( out, 20, "%d\t%d\t%x\r\n", i+1, j+1, pcData[i][j] );
            vConsolePrint( out );
        }
    }
}

static void vPayloadPrototypePollData()
{
	/*TODO
    char pcPayloadData[payloadCHANNELS*5];
    char cPayloadReg = payloadREGISTER;
    unsigned char i;

    for( i=0; i<payloadCHANNELS; ++i )
    {
        pcPayloadData[i] = 0;
    }

    vConsolePrint( "Polling Prototype Payload\r\n" );
    vTaskDelay(100);
    
    if(wireSTATUS_SUCCESS != cWireWritePutsError( payloadBUS_PROTOTYPE, payloadADDR_PROTOTYPE, &cPayloadReg, 1 ) )
    {
        vConsoleErrorPrintf( "Payload: I2C Write Error\r\n" );
        return;
    }
    if(wireSTATUS_SUCCESS != cWireReadPutsError( payloadBUS_PROTOTYPE, payloadADDR_PROTOTYPE, pcPayloadData, payloadCHANNELS*5 ) )
    {
        vConsoleErrorPrintf( "Payload: I2C Read Error\r\n" );
        return;
    }

    vConsolePrint( "Chan\tValue\r\n" );
    for( i=0; i<payloadCHANNELS*5; ++i )
    {
        vTaskDelay(10);
        vConsolePrintf( "%d\t%x\r\n", i+1, 0xff & pcPayloadData[i] );
    }
    */
}

static void vPayloadPollData()
{
	/* TODO
    char pcPayloadData[payloadRADIOMETERS][payloadCHANNELS];
    char cPayloadReg = payloadREGISTER;

    vConsolePrint( "Polling Payload." );

    // Turn on payload and wait one second before communicating
    payloadENABLE1_TRIS = 1;
    vTaskDelay(1000);
    cWireWritePutsError( payloadBUS_PROTOTYPE, payloadADDR_PROTOTYPE, &cPayloadReg, 1 );
    cWireReadPutsError( payloadBUS1, payloadADDR, pcPayloadData[0], payloadCHANNELS );
    payloadENABLE1_TRIS = 0;

    payloadENABLE2_TRIS = 1;
    vTaskDelay(1000);
    cWireWritePutsError( payloadBUS_PROTOTYPE, payloadADDR_PROTOTYPE, &cPayloadReg, 1 );
    cWireReadPutsError( payloadBUS2, payloadADDR, pcPayloadData[1], payloadCHANNELS );
    payloadENABLE2_TRIS = 0;

    payloadENABLE3_TRIS = 1;
    vTaskDelay(1000);
    cWireWritePutsError( payloadBUS_PROTOTYPE, payloadADDR_PROTOTYPE, &cPayloadReg, 1 );
    cWireReadPutsError( payloadBUS3, payloadADDR, pcPayloadData[2], payloadCHANNELS );
    payloadENABLE3_TRIS = 0;

    // Print payload data locally
    vPayloadPrintData( pcPayloadData );
	 */
    /* TODO Store data to memory */
}

static void vPayloadTask( void *pvParameters )
{
    for( ;; )
    {
        vTaskDelay(5000);

        if(1) //check GPIO PIN here for image stored
        {
         if(vPayloadGetImageOnSDCard())
         {
        	 //post PostPayloadEvent
         }
        }

//        vWireScan( payloadBUS_PROTOTYPE );
        //vPayloadPrototypePollData();
//        vPayloadPollData();
    }
}
static int vPayloadGetImageOnSDCard()
	{

				FATFS FatFs;   /* Work area (file system object) for logical drive */
			    FIL file;       /* File object */
			    FRESULT fr;    /* FatFs return code */
			    BYTE buffer[512];    /* was 4096 File copy buffer */
			    char file_name_buffer[20];
			    UINT br;

			 /* Register work area to the default drive **/
			     f_mount(&FatFs,"", 0);
			 /* Open the file */
			 strcpy(file_name_buffer, vPayloadGetImageFileName(file_name_buffer));

			 if(file_name_buffer  == NULL)
				 return PAYLOAD_ERROR;

		    fr = f_open(&file, file_name_buffer, FA_READ);

		    if (fr)
		    	return (int) PAYLOAD_ERROR;

		    for (;;)
		    		{
		            	fr = f_read(&file, buffer, 256, &br);  /* Read a chunk of image file */

		            	if (fr || br == 0)
		            	{
		            		return PAYLOAD_ERROR; //break; /* error or eof */
		            	}
		            	f_close(&file);
		           	 }
		    /* Close the file */
		    f_close(&file);

		    return PAYLOAD_OK;


}

static int vPayloadWriteToSDCard(char* file_name,char* data_string)
{

	UINT bw;

			/** Read a text file and display it */

			FATFS FatFs;   /* Work area (file system object) for logical drive */
		    FIL file1;       /* File object */



			     /* Register work area to the default drive **/
			    f_mount(&FatFs,"", 0);
			    /* Open the file */

			    if (f_open(&file1, file_name, FA_WRITE | FA_OPEN_ALWAYS) == FR_OK)
			    {	// Open existing or create new file
			    		if (f_lseek(&file1, f_size(&file1)) == FR_OK)
			    			{
			    				char *text2 = "This is a new line, appended to existing file!\r\n";
			    				f_write(&file1, text2, strlen(text2), &bw); // Write data to the file
			    				if (bw == strlen(text2))
			    				{ //we wrote the entire string
			    					vConsolePrintf("Appended:%d\n", (int)f_size(&file1));
			    				}
			    			}
			    	f_close(&file1);// Close the file
			    	return PAYLOAD_OK;
			    }
			    else
			    	return PAYLOAD_ERROR;
}

static char* vPayloadGetImageFileName(char* file_name_buffer)
{
//use the elapsed time in seconds since launch to get the latest file time stamp.
//or retrieve file names from a file.

	FATFS FatFs;   /* Work area (file system object) for logical drive */
	FIL file;       /* File object */
	FRESULT fr;    /* FatFs return code */
	UINT br;
 /* Register work area to the default drive **/
     f_mount(&FatFs,"", 0);
 /* Open the file */
    fr = f_open(&file, "log.txt", FA_READ);

    if (fr)
    return NULL;

	   for (;;)
	   		{
	           	fr = f_read(&file, file_name_buffer, 16, &br);  /* Read file name */
	            	if (fr || br == 0) /* error or eof */
	            	{
	            		f_close(&file);
	           		   return NULL; //break;
	            	}

	      	 }
		    /* Close the file */
		    f_close(&file);

return file_name_buffer;

}
