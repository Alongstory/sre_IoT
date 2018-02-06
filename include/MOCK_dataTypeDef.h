/********************************************************************************
 * Schneider-Electric                                                           *
 * Global Solutions - Digital Services Transformation                           *
 * Digital Services Platform                                                    *
 * Copyright (c) 2014 - All rights reserved.                                    *
 *                                                                              *
 * Developed by ESA Team :                                                      *
 * 			Hicham Hossayni (SESA377732)                                       *
 * 			Imran Khan		(SESA388470)                                       *
 *                                                                              *
 * No part of this document may be reproduced in any form without the           *
 * express written consent of Schneider-Electric.                               *
 ********************************************************************************/

/**
 * @file
 *
 * @author Hicham Hossayni (SESA377732)
 */

#ifndef MOCK_DATATYPEDEF_H_
#define MOCK_DATATYPEDEF_H_

#ifdef __cplusplus
extern "C" {
#endif

struct MOCK_Tag ;
struct MOCK_Variable;
struct MOCK_Device;

typedef struct MOCK_Tag 		MOCK_Tag;
typedef struct MOCK_Variable 	MOCK_Variable;
typedef struct MOCK_Device 		MOCK_Device;

typedef enum
{
	MOCK_TYPE_FLOAT   = 0x01,
	MOCK_TYPE_LONG    = 0x02,
	MOCK_TYPE_INT     = 0x03,
	MOCK_TYPE_CHAR    = 0x04,
	MOCK_TYPE_STR     = 0x05,     /* string type */
	MOCK_TYPE_BOOL    = 0x06,     /* boolean type */
	MOCK_TYPE_INT_16  = 0x07,     /* signed int_16 */
	MOCK_TYPE_UINT_8  = 0x08,     /* unsigned int_8 */
	MOCK_TYPE_UINT_16 = 0x09,
	MOCK_TYPE_UINT_32 = 0x0A,
	MOCK_TYPE_UINT_64 = 0x0B,
	MOCK_TYPE_BLOB    = 0x0C      /* BLOB type, the size of this type must be specified in the s_Variable structure */
}MOCK_variable_type;
/*
 * structure of a tag
 * NOTE: All pointers must be allocated (or NULL) by the implementer and free by the implementer
 */
struct MOCK_Tag
{
	char *  	Namespace;
	char *		instance;

	MOCK_Tag * 	next;
};

/*
 * structure of a variable
 * NOTE: All pointers must be allocated (or NULL) by the implementer and free by the implementer
 */
struct MOCK_Variable
{
	char *			id;
	char *			name;
	char *  		unit;
	char *  	    parent_deviceId; /*the device where the variable is attached */
	MOCK_Tag *			tags;			/* list of variable's tags */

	MOCK_variable_type 	type;
	void *  	    value;

	MOCK_Variable * 	next;
};


/*
 * type of a device
 * NOTE: All pointers must be allocated (or NULL) by the implementer and free by the implementer
 */
struct MOCK_Device
{
	char *			id;				/* identifier of the device */
	char *  		name;			/* name of the device */
	char *          parentId;      	/* ID of the device's parent */
	MOCK_Tag *			tags;			/* list of device's tags */
	MOCK_Variable *	variables;
	MOCK_Device * 		children;		/* list of device's children (must be devices) */
	MOCK_Device * 		next;			/* next device (brother) */

};

#ifdef __cplusplus
}
#endif


#endif /* MOCK_DATATYPEDEF_H_ */
