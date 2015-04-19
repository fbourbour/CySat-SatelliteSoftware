
/* 
 * File:   helium.h
 * Author: Kris
 *
 * Created on February 23, 2013, 12:59 PM
 * Ported to Visual C++ for simulation: Fred Bourbour 1/10/2014
 */


typedef unsigned char uint8_t;
typedef unsigned int uint16_t;


#define heliumQUEUE_SIZE  ( (unsigned short) 20 )

/* Sync Characters */
#define SYNC_1 'H'
#define SYNC_2 'e'

/* Input/Output message types */
#define MESSAGE_I                 0x10  /* To radio */
#define MESSAGE_O                 0x20  /* From radio */

/* Configuration commands (yes, it is supposed to skip 0x0A - 0x0F) */
#define COMMAND_NO_OP             0x01  /* No operation */
#define COMMAND_RESET             0x02  /* Reset the system */
#define COMMAND_TRANSMIT          0x03  /* Transmit bytes to radio board */
#define COMMAND_RECEIVE           0x04  /*  */
#define COMMAND_GET_TRANS_CONFIG  0x05  /* Read the radio configuration */
#define COMMAND_SET_TRANS_CONFIG  0x06  /* Set the radio configuration */
#define COMMAND_QUERY_TELEMETRY   0x07  /* Get a radio telemetry frame */
#define COMMAND_WRITE_FLASH       0x08  /* Write the flash with the MD5 checksum */
#define COMMAND_RF_CONFIG         0x09  /* Access the low level RF config */
#define COMMAND_BEACON_DATA       0x10  /* Set the contents of the beacon */
#define COMMAND_BEACON_CONFIG     0x11  /* Configure the beacon */
#define COMMAND_READ_FIRM_REV     0x12  /* Read the radio's firmware revision */
#define COMMAND_DIO_KEY_WRITE     0x13  /* Write to the digital output pin */
#define COMMAND_FIRMWARE_UPDATE   0x14  /* Update the radio firmware */
#define COMMAND_FIRMWARE_PACKET   0x15  /* A packet containing the firmware */
#define COMMAND_PA_SET            0x20  /* High speed setting of PA level */

#define HEADER_SIZE       8
#define PAYLOAD_MAX_SIZE  225
#define PACKET_MAX_SIZE   ( HEADER_SIZE + PAYLOAD_MAX_SIZE + 2 )

#define heliumACK  0x0A0A
#define heliumNACK 0xFFFF


typedef struct {
    /* Command type */
    uint8_t ucCommand;
    /* Actual Payload Size (factoring out ACK, etc) */
    uint16_t usPayloadSize;
    /* ACK field (from overloaded payload field in packet)*/
    uint16_t usAck;
    /* Packet data array */
    uint8_t *pucPayload;
} heliumPacket;





void he_generate_packet_header(const char* command, short payload_length, char* header);
short he_calculate_checksum(const char* data, int start, int end);
//uint8_t* vHeliumSendPacket( heliumPacket *pxPacket );
void vHeliumSendPacket( heliumPacket *pxPacket );
void vHeliumSendData( char *pcData, uint16_t usSize );
static void vCalculateChecksum( uint8_t *pucData, uint16_t usSize, uint8_t *pucSumA, uint8_t *pucSumB );
