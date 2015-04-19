#include "stdafx.h"
#include "HeliumSim.h"


/* 
 * File:   helium.h
 * Author: Kris
 *
 * Created on February 23, 2013, 12:59 PM
 * Ported to Visual C++ for simulation: Fred Bourbour 1/10/2014
 */

extern CString HeliumData;


/*
 * Generate the header for a packet
 *
 * @param command The command to send the radio
 * @param payload_length The size (in bytes) of the data payload
 * @param header The array to store the 8 byte header in
 */
void he_generate_packet_header(const char* command, short payload_length, char* header) {
	// Variable to hold the calculated checksum
	short check = 0;

	// Package the sync characters
	header[0] = 0x48;
	header[1] = 0x65;

	// Place the command into the radio
	header[3] = *command;

	// Package the payload length in the packet
	header[5] = payload_length;

	// Calculate the header's checksum and store it in the header
	check = he_calculate_checksum(header, 2, 6);
	header[7] = check;
}


/*
  * Calculate the 8-bit Fletcher checksum for the header
  *
  * @param header The data to calculate over
  * @param start The first index to include in checksum
  * @param end The last index to include in checksum
  * @return The two byte checksum value
  */
short he_calculate_checksum(const char* data, int start, int end) {
	int sum_a = 0, sum_b = 0;
	int cnt = 0;

	for(cnt=start; cnt<end; cnt++) {
		sum_a = (sum_a + data[cnt]) % 255;
		sum_b = (sum_b + sum_a) % 255;
	}

	return(sum_a << 8 | sum_b);
}

void vHeliumSendData( char *pcData, uint16_t usSize )
{
    heliumPacket xPacket;

    xPacket.ucCommand = COMMAND_TRANSMIT;
    xPacket.usPayloadSize = usSize;
    xPacket.pucPayload = (uint8_t*) pcData;

//    vHeliumSendPacket( &xPacket );
}

uint8_t* vHeliumSendPacket( heliumPacket *pxPacket )
{
    
    CString helium_data="";
	CString test ="";
	HeliumData ="";
    uint8_t pucBuffer[PACKET_MAX_SIZE];
    uint16_t length,i, size = HEADER_SIZE;

	memset(pucBuffer, 0x00, sizeof(pucBuffer));

    pucBuffer[0] = SYNC_1;
    pucBuffer[1] = SYNC_2;
    pucBuffer[2] = MESSAGE_I;
    pucBuffer[3] = pxPacket->ucCommand;
    pucBuffer[4] = (uint8_t) ( ( pxPacket->usPayloadSize ? pxPacket->usPayloadSize : pxPacket->usAck ) >> 8 );
    pucBuffer[5] = (uint8_t) ( ( pxPacket->usPayloadSize ? pxPacket->usPayloadSize : pxPacket->usAck ) & 0xFF );


    for(uint8_t j = 0; j < HEADER_SIZE; j++)
	{
      if(pucBuffer[j] >= 0 && pucBuffer[j] <= 9)
		  pucBuffer[j] += 0x30;
	}

    vCalculateChecksum( &pucBuffer[2], 4, &pucBuffer[6], &pucBuffer[7] );

    if( 0 != pxPacket->usPayloadSize )
    {
        size += pxPacket->usPayloadSize + 2;

        if( size > PACKET_MAX_SIZE || NULL == pxPacket->pucPayload )
        {
            /* Bad input */
            /* TODO Error message */
            return(pucBuffer);
        }

        for( i=0; i<pxPacket->usPayloadSize; ++i )
        {
            pucBuffer[i+HEADER_SIZE] = pxPacket->pucPayload[i];
        }

          for(j = 0; j < HEADER_SIZE + pxPacket->usPayloadSize; j++)
		  {
           if(pucBuffer[j] >= 0 && pucBuffer[j] <= 9)
		   pucBuffer[j] += 0x30;
		  }

        vCalculateChecksum( &pucBuffer[2], HEADER_SIZE-2 + pxPacket->usPayloadSize, &pucBuffer[size-2], &pucBuffer[size-1] );
    }
 

 length = heliumQUEUE_SIZE + pxPacket->usPayloadSize;
 for(i=0; i<length; i++)
 {
	 
//  if(pucBuffer[i] >= 0 && pucBuffer[i] <= 9 )
//      pucBuffer[i] += 0x30;
//    helium_data += 0x30;
//  else
   helium_data.Format("%c",pucBuffer[i]);
  HeliumData += helium_data;
  test = HeliumData;
 }
return(pucBuffer);

    for( i=0; i<size; ++i )
    {
        /* TODO Check for failure */
        //ucHeliumSendUart( pucBuffer[i] );
    }

}

static void vCalculateChecksum( uint8_t *pucData, uint16_t usSize, uint8_t *pucSumA, uint8_t *pucSumB )
{
    uint16_t i;
    uint8_t test;

    *pucSumA = 0;
    *pucSumB = 0;

    for( i = 0; i < usSize; i++ ) //was i = 2 FBDEBUG
    {
       test = pucData[i];

	    //if(test == 0)
		//	continue;

        *pucSumA += pucData[i];
        *pucSumB += *pucSumA;
    }
}
