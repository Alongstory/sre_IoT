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


#ifndef DMAPI_FUNCTIONS_H_
#define DMAPI_FUNCTIONS_H_


#include "MOCK_dataTypeDef.h"
#include "dmapi.h"

#ifdef __cplusplus
extern "C" {
#endif


 /**
 * @brief creates a new device from the input, all pointers are initialized to NULL
 *
 * @param the device's id and name
 *
 * @return a _Device structure
 */
s_Device * DM_NewDevice(char *id, char * name, int * status);


/**
* @brief creates a new variable value from the input, must evolve to integrate all defined types
*        for the moment, just few types are supported
*
* @param the variable's type and its value
*
* @return a void pointer on the value memory
*/
void * DM_NewVariableValue(variable_type type, void * value, int * status);


/**
* @brief creates a new variable from the input, all pointers are initialized to NULL
*
* @param the variable's id, name, unit, type and value
*
* @return a _Variable structure
*/
s_Variable * DM_NewVariable(char *id, char * name, char * unit, variable_type type, void * value, int * status);



/**
* @brief creates a new NodeId from the input, the next pointer is initialized to NULL
*        I recall that the node can be either a device or a variable
*
* @param the node's ID
*
* @return a s_NodeId structure
*/
s_NodeId * DM_NewNodeId(char *id, int * status);


/**
* @brief creates a new tag from the input, the next pointer is initialized to NULL
*
* @param the tag's namespace and instance
*
* @return a _Tag structure
*/
s_Tag * DM_NewTag(char *Namespace, char * instance, int * status);


s_Device * DM_getDevicesFromMockDevices(MOCK_Device * mdevices, int * status);

s_NodeId * DM_getDevicesIDsFromMockDevices(MOCK_Device * mdevices, int * status);

s_Variable * DM_getVariableFromMockVariable(MOCK_Variable * mvariable, int * status);

s_Variable * DM_getVariablesFromMockVariables(MOCK_Variable * mvariables, int * status);

s_NodeId * DM_getVariablesIDsFromMockVariables(MOCK_Variable * mvariables, int * status);



s_Tag * DM_getTagsFromMockTags(MOCK_Tag * mtags, int * status);

MOCK_Tag * DM_getMockTagsFromTags(s_Tag * tags, int * status);

//----------------- DEBUG FUNCTIONS --------------------------

void DM_print_device(s_Device * device, int level);
void DM_print_devicesList(s_Device * devices, int level);
void DM_print_tag(s_Tag * tag, int level);
void DM_print_tagsList(s_Tag * tagsList, int level);
void DM_print_variable(s_Variable * variable, int level);
void DM_print_variablesList(s_Variable * variablesList, int level);

#ifdef __cplusplus
}
#endif

#endif /* DMAPI_FUNCTIONS_H_ */
