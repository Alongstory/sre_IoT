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


#ifndef MOCK_H_
#define MOCK_H_

#include "MOCK_dataFunctions.h"
#include "MOCK_dataTypeDef.h"


#ifdef __cplusplus
extern "C" {
#endif

extern short motorValue;


MOCK_Device * getMockTree(int * status);

void  freeMockTree();


//------------------  some debug functions ---------------------

/**
 * print a device with all its tags, variables, variables tags, and children devices
 */
void MOCK_print_device(MOCK_Device * device, int level);

/**
 * like print_device but prints also the brothers.
 */
/*
void MOCK_print_devicesList(MOCK_Device * devices, int level);

void MOCK_print_tag(MOCK_Tag * tag, int level);

void MOCK_print_tagsList(MOCK_Tag * tagsList, int level);

void MOCK_print_variable(MOCK_Variable * variable, int level);

void MOCK_print_variablesList(MOCK_Variable * variablesList, int level);
*/


//----------------- Search functions ------------------------------------


/**
 * @brief Return the Device from the input id
 *
 * @param the tree where to search the device and the id
 *
 * @return the pointer of the device (if found) on the tree
 */

MOCK_Device * MOCK_getDeviceById(MOCK_Device * tree , char * id);



/**
 * @brief Return the Variable with id = id and which parent (ancestor) is the input device
 *
 * @param the the device's parent structure and the variable's id
 *
 * @return the pointer of the variable (if found) on the tree
 */
MOCK_Variable * MOCK_getVariableById(MOCK_Device * device, char * id);



/**
 * @brief verify if a device has the same tag as input
 *
 * @param the device and tag structures
 *
 * @return true if the device has the tag, otherwise false
 */
int MOCK_isDeviceHasTag(MOCK_Device * device, MOCK_Tag * tag);


/**
 * @brief verify if a device has at least one of the provided tags
 *
 * @param the device and list of tags
 *
 * @return true if the device has one of the input tags, otherwise false
 */
int MOCK_isDeviceHasTags(MOCK_Device * device, MOCK_Tag * tags);


/**
 * @brief verify if a variable has the same tag as input
 *
 * @param the variable and tag structures
 *
 * @return true if the variable has the tag, otherwise false
 */
int MOCK_isVariableHasTag(MOCK_Variable * variable, MOCK_Tag * tag);


/**
 * @brief verify if a variable has at least one of the provided tags
 *
 * @param the variable and tag structures
 *
 * @return true if the variable has one of the input tags, otherwise false
 */
int MOCK_isVariableHasTags(MOCK_Variable * variable, MOCK_Tag * tags);


/**
 * @brief Return all devices having at least one of the input tags
 *
 * @param the results pointer, the tree where to search devices, and the list of tags
 *
 * @return the number of devices found
 */

int MOCK_getDevicesByTags(MOCK_Device * results, MOCK_Device * tree , MOCK_Tag * tags);


/**
 * @brief Return all variables having at least one of the input tags
 *
 * @param the results pointer, the device tree where to search variables, and the list of tags
 *
 * @return the number of variables found
 */

int MOCK_getVariablesByTags(MOCK_Variable * results, MOCK_Device * tree , MOCK_Tag * tags);


#ifdef __cplusplus
}
#endif


#endif /* MOCK_H_ */
