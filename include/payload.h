/*
 * File:   payload.h
 * Author: Kris Scott
 *
 * Created on October 30, 2012, 2:51 PM
 */

#ifndef PAYLOAD_H
#define	PAYLOAD_H

#include <GenericTypeDefs.h>

void vPayloadStartTask();


#define IMAGE_SLOT_SIZE 16384 //16k


typedef enum
{
	IMAGE_STORED_TO_SD,
	IMAGE_LOADED_TO_SLOT, /* Attribute: Slot index */
	IMAGE_FLUSHED_FROM_SLOT /* Attribute Slot index */

}PayloadSig_t;

typedef enum
{
FLUSH_IMAGE, /* Attribute: Slot index */
ACTIVATE_PAYLOAD,
PURGE_SD_CARD,
SET_CAPTURE_FREQUENCY, /* attribute: Frequency enum */
SET_IMAGE_RESOLUTION   /* attribute : resolution enum */
}CommunicationsSig_t;

enum ImageSlotStatus
{
	IMAGE_SLOT_FLUSHED,
	IMAGE_SLOT_FULL
};


typedef struct
{
PayloadSig_t sig;
uint32_t attribute;
}PayloadEvent_t;

typedef struct
{
CommunicationsSig_t sig;
uint32_t attribute;
}CommunicationsEvent_t;


enum PAYLOAD_BOARD_STATES
{
	UNINTIALIZED, // The payload board begins in this state. variables undefined the payload board is undefined
	INACTIVE, // Payload board is in low power mode. No SD operations are performed. Safe mode
	ACTIVE // Capturing images and putting them onto the SD card.
};

enum PAYLOAD_STATUS
{
	PAYLOAD_OK,
	PAYLOAD_ERROR,
};
typedef struct
{
uint16_t ImageSize;
uint8_t SlotStatus;
uint8_t Image[IMAGE_SLOT_SIZE];
}ImageSlot_t;

#define NUMBER_OF_SLOTS 2
ImageSlot_t imageSlot[NUMBER_OF_SLOTS];

#endif	/* PAYLOAD_H */

