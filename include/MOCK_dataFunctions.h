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

#ifndef DMNODESSTRUCTURE_H_
#define DMNODESSTRUCTURE_H_

#include "MOCK_dataTypeDef.h"

#ifdef __cplusplus
extern "C" {
#endif



/**  Functions  */



//----------------- Nodes creation --------------------------


 /**
 * @brief creates a new device from the input, all pointers are initialized to NULL
 *
 * @param the device's id and name
 *
 * @return a _Device structure
 */
MOCK_Device *   	MOCK_NewDevice  ( char *id, char * name ,int * status);


/**
* @brief creates a new variable from the input, all pointers are initialized to NULL
*
* @param the variable's id, name, unit, type and value
*
* @return a MOCK_Variable structure
*/

MOCK_Variable * MOCK_NewVariable  ( char *id, char * name, char * unit, MOCK_variable_type type, void * value,int * status);


/**
* @brief creates a new tag from the input, the next pointer is initialized to NULL
*
* @param the tag's namespace and instance
*
* @return a MOCK_Tag structure
*/
MOCK_Tag * MOCK_NewTag  ( char *Namespace, char * instance,int * status);


//----------------- Nodes cloning --------------------------


 /**
 * @brief clone a device node and all its tags
 *
 * @param the device's pointer
 *
 * @return a new _Device structure which is the copy of the input node with all its tags
 */
MOCK_Device * MOCK_CloneDevice(MOCK_Device * device,int * status);


/**
* @brief clone a variable node and all its tags
*
* @param the variables's pointer
*
* @return a new _Variable structure which is the copy of the input node with all its tags
*/

MOCK_Variable * MOCK_CloneVariable(MOCK_Variable* variable,int * status);


/**
* @brief clone a tag node
*
* @param the tag's pointer
*
* @return a new _Tag structure which is the copy of the input node
*/
MOCK_Tag * MOCK_CloneTag(MOCK_Tag * tag,int * status);



/**
* @brief clone a tag node and all its brothers in the ->next pointer
*
* @param the first tag's pointer in the list
*
* @return a pointer on the first tag structure of the list copy
*/
MOCK_Tag * MOCK_CloneAllTags(MOCK_Tag *tag,int * status);



MOCK_Variable * MOCK_CloneAllVariables(MOCK_Variable* listVariables,int * status);


//----------------- Device nodes functions --------------------------

/**
* @brief adds a device node to another device node
*
* @param the device and its new parent pointers
*
* @return 0 for success and -1 for failure
*/
int MOCK_AddDeviceToDevice  ( MOCK_Device * parent, MOCK_Device * device);


/**
* @brief adds a tag node to a device node
*
* @param the device's a tag's pointers
*
* @return 0 for success and -1 for failure
*/
int MOCK_AddTagToDevice  ( MOCK_Device * device, MOCK_Tag * tag);


/**
* @brief adds a variable node to a device node
*
* @param the device's a variable's pointers
*
* @return 0 for success and -1 for failure
*/
int MOCK_AddVariableToDevice  ( MOCK_Device * device, MOCK_Variable * variable);


/**
* @brief free recursively the memory of a device node and all its children
*
* @param the device's pointer and its parent's node
*
* @return 0 for success and -1 for failure
*/
int MOCK_DeleteDevice  ( MOCK_Device * device);


//----------------- Variable nodes functions --------------------------

/**
* @brief adds a tag node to a variable node
*
* @param the variables's a tag's pointers
*
* @return 0 for success and -1 for failure
*/
int MOCK_AddTagToVariable  ( MOCK_Variable * variable, MOCK_Tag * tag);


/**
* @brief free the memory of a variable node
*
* @param the variables's pointer and its parent's node
*
* @return 0 for success and -1 for failure
*/
int MOCK_DeleteVariable  ( MOCK_Variable * variable);



/**
* @brief free the memory of a list variable nodes
*
* @param the first variables's pointer
*
* @return 0 for success and -1 for failure
*/
int MOCK_DeleteAllVariables  ( MOCK_Variable * variables);


int MOCK_DeleteAllDevices  ( MOCK_Device * devices);

int MOCK_DeleteAllTags  ( MOCK_Tag * tags);


int MOCK_DeleteTag(MOCK_Tag * tag);

#ifdef __cplusplus
}
#endif


#endif /* DMNODESSTRUCTURE_H_ */
