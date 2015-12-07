#pragma once

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#define CMD_REGISTER_CLIENT					0x01 
#define CMD_RETRIEVE_STOCK_OF_CLIENT		0x02 
#define CMD_ADVERTISING						0x03 
#define CMD_BID								0x04 
 
#define RSP_REGISTER_CLIENT					0x81 
#define RSP_RETRIEVE_STOCK_OF_CLIENT		0x82 
#define RSP_ADVERTISING						0x83 
#define RSP_BID								0x84 

#define CMD_BROADCAST_PRICE					0xF1  
#define CMD_BROADCAST_STATUS			    0xF2 

typedef enum
{
	E_NONE 	 	  = 0,
	E_ADVERTISING = 1,
	E_AUCTION	  = 2,
} EAuctionState;


#endif //__PROTOCOL_H__