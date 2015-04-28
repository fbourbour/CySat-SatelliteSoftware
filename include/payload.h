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
#define PAYLOAD_ERROR 1
#define PAYLOAD_OK 0
#define IMAGE_SLOT_SIZE 65536

typedef enum
{
	IMAGE_STORED_TO_SD,
	IMAGE_LOADEDTO_SLOT, /* Attribute: Slot index */
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

typedef struct
{
uint16_t ImageSize;
uint8_t Image[IMAGE_SLOT_SIZE];
}ImageSlot_t;

#define NUMBER_OF_SLOTS 2
extern ImageSlot_t imageSlot[NUMBER_OF_SLOTS];

#endif	/* PAYLOAD_H */

