#ifndef PUBLIC_INCLUDE_IOTHUB_AGENT_IOTHUBAGENT_DM_H_
#define PUBLIC_INCLUDE_IOTHUB_AGENT_IOTHUBAGENT_DM_H_

#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#include "iothubAgent_errorCodes.h"

#ifdef __cplusplus
extern "C" {
#endif

//
// Device stuff
//

/**
 * @ingroup dm
 * Callback triggered on reboot request from the hub.
 * @return IOTHUBAGENT_OK if the request is accepted an @ref iothubAgent_Error if not.
 */
typedef iothubAgent_Error (*iothubAgent_executeRebootCbk)();

/**
 * @ingroup dm
 * Callback triggered on factory reset request from from the hub.
 * @return IOTHUBAGENT_OK if the request is accepted an @ref iothubAgent_Error if not.
 */
typedef iothubAgent_Error (*iothubAgent_executeFactoryResetCbk)();

/**
 * @ingroup dm
 * Callback triggered on time read request from the hub..
 * @return The posix epoch time of the device.
 */
typedef time_t  (*iothubAgent_readCurrentTimeCbk)();

/**
 * @ingroup dm
 * Callback triggered on time write request from the hub.
 * @param [in] p_epochTime The time to set, since posix epoch, in the device.
 * @return IOTHUBAGENT_OK if the request is accepted an @ref iothubAgent_Error if not.
 */
typedef iothubAgent_Error (*iothubAgent_writeCurrentTimeCbk)(time_t p_epochTime);

/**
 * @ingroup init
 * Structure holding all DM related callbacks.
 */
typedef struct {
  iothubAgent_executeRebootCbk executeRebootCbk;              ///< Callback called upon reboot request
  iothubAgent_executeFactoryResetCbk executeFactoryResetCbk;  ///< Callback called upon factory reset request
  iothubAgent_readCurrentTimeCbk readCurrentTimeCbk;          ///< Callback called upon device current time request
  iothubAgent_writeCurrentTimeCbk writeCurrentTimeCbk;        ///< Callback called upon device current time setting request
} iothubAgent_deviceCbks;

/**
 * @ingroup dm
 * Set the device manufacturer presented in DM messages.
 * @param [in] p_manufacturer A null terminated string representing the manufacturer name.
 *             The string is copied and can be safely freed after function call.
 * @return IOTHUBAGENT_OK on success an @ref iothubAgent_Error else where.
 */
iothubAgent_Error iothubAgent_device_setManufacturer(const char* p_manufacturer);

/**
 * @ingroup dm
 * Set the device model number presented in DM messages.
 * @param [in] p_modelNumber A null terminated string representing the model number.
 *             The string is copied and can be safely freed after function call.
 * @return IOTHUBAGENT_OK on success an @ref iothubAgent_Error else where.
 */
iothubAgent_Error iothubAgent_device_setModelNumber(const char* p_modelNumber);

/**
 * @ingroup dm
 * Set the device serial number presented in DM messages.
 * @param [in] p_serialNumber A null terminated string representing the serial number.
 *             The string is copied and can be safely freed after function call.
 * @return IOTHUBAGENT_OK on success an @ref iothubAgent_Error else where.
 */
iothubAgent_Error iothubAgent_device_setSerialNumber(const char *p_serialNumber);

/**
 * @ingroup dm
 * Set the device firmware version presented in DM messages.
 * @param [in] p_firmwareVersion A null terminated string representing the serial number.
 *             The string is copied and can be safely freed after function call.
 * @return IOTHUBAGENT_OK on success an @ref iothubAgent_Error else where.
 */
iothubAgent_Error iothubAgent_device_setFirmwareVersion(const char *p_firmwareVersion);

//
// Firmware stuff
//

/**
 * @ingroup dm
 * @enum iothubAgent_firmwareUpdateState
 * Firmware update state, mapped on LWM2M Update State -- for /5/0/3 resource.
 * This enum represents the status of the firmware download process @b only
 */
typedef enum  iothubAgent_firmwareUpdateState{
  IOTHUBAGENT_UPDATE_STATE_NONE = 1,              ///< State is unknown (most likely not initialized)
  IOTHUBAGENT_UPDATE_STATE_DOWNLOADING_IMAGE = 2, ///< The device has started an image download
  IOTHUBAGENT_UPDATE_STATE_IMAGE_DOWNLOADED = 3   ///< The device has finished a firmware download
} iothubAgent_firmwareUpdateState;

/**
 * @ingroup dm
 * Firmware update status, mapped on LWM2M Update Result -- for /5/0/5 resource
 * This enum represents the status of the firmware update process @b only
 */
typedef enum {
  IOTHUBAGENT_UPDATE_RESULT_DEFAULT = 0,            ///< Update result is unknown (most likely not initialized)
  IOTHUBAGENT_UPDATE_RESULT_UPDATE_SUCCESSFUL = 1,  ///< Update is successful
  IOTHUBAGENT_UPDATE_RESULT_NOT_ENOUGH_STORAGE = 2, ///< Update fails, no more space
  IOTHUBAGENT_UPDATE_RESULT_OUT_OF_MEMORY = 3,      ///< Update fails not enough memory (memory is not space !)
  IOTHUBAGENT_UPDATE_RESULT_CONNECTION_LOST = 4,    ///< Update fails
  IOTHUBAGENT_UPDATE_RESULT_CRC_FAILURE = 5,        ///< Update fails during CRC check
  IOTHUBAGENT_UPDATE_RESULT_UNSUPPORTED_PACKAGE = 6,///< Update fails invalid package format
  IOTHUBAGENT_UPDATE_RESULT_INVALID_URI = 7         ///< Update fails invalid package URI
} iothubAgent_firmwareUpdateResult;

/**
 * @ingroup dm
 * Callback triggered when the DSP service notifies the availability of a new firmware.
 * <b>Firmware download must not be done within this callback.</b>
 * @param p_pkgUri The URI where to find the new firmware.
 * @return IOTHUBAGENT_OK if the new package is taken into account (for further download) or a @ref iothubAgent_Error.
 */
typedef iothubAgent_Error (*iothubAgent_writePackageUriCbk)(const char* p_pkgUri);

/**
 * @ingroup dm
 * Callback triggered when the hub notifies the device to perform a firmware update.
 * <b>Firmware update must not be done within this callback.</b>
 * @return IOTHUBAGENT_OK if the upgrade command is taken into account or a @ref iothubAgent_Error.
 */
typedef iothubAgent_Error (*iothubAgent_executeUpdateCbk)();

/**
 * @ingroup init
 * @ingroup dm
 * Structure holding callbacks related to firmware object.
 */
typedef struct {
  iothubAgent_writePackageUriCbk writePackageUriCbk;  ///< Call back triggered to notify availability of a new firmware
  iothubAgent_executeUpdateCbk executeUpdateCbk;      ///< Call back triggered to start a firmware update
} iothubAgent_firmwareUpdateCbks;

/**
 * @ingroup dm
 * Set the firmware update state.
 * @param [in] p_sate on of the @ref iothubAgent_firmwareUpdateState.
 * @return IOTHUBAGENT_OK.
 */
iothubAgent_Error iothubAgent_firmwareUpdate_setState(iothubAgent_firmwareUpdateState p_sate);

/**
 * @ingroup dm
 * Set the firmware update result.
 * @param [in] p_updateResult one of @ref iothubAgent_firmwareUpdateResult.
 * @return IOTHUBAGENT_OK.
 */
iothubAgent_Error iothubAgent_firmwareUpdate_setUpdateResult(iothubAgent_firmwareUpdateResult p_updateResult);

//
// Network monitoring stuff
//

/**
 * @ingroup dm
 * Network bearer list.
 */
typedef enum {
 IOTHUBAGENT_NETWORK_BEARER_GSM_CELLULAR_NETWORK = 0,
 IOTHUBAGENT_NETWORK_BEARER_TD_SCDMA_CELLULAR_NETWORK = 1,
 IOTHUBAGENT_NETWORK_BEARER_WCDMA_CELLULAR_NETWORK = 2,
 IOTHUBAGENT_NETWORK_BEARER_CDMA2000_CELLULAR_NETWORK = 3,
 IOTHUBAGENT_NETWORK_BEARER_WIMAX_CELLULAR_NETWORK = 4,
 IOTHUBAGENT_NETWORK_BEARER_LTE_TDD_CELLULAR_NETWORK = 5,
 IOTHUBAGENT_NETWORK_BEARER_LTE_FDD_CELLULAR_NETWORK = 6,
 IOTHUBAGENT_NETWORK_BEARER_WLAN_NETWORK = 21,
 IOTHUBAGENT_NETWORK_BEARER_BLUETOOTH_NETWORK = 22,
 IOTHUBAGENT_NETWORK_BEARER_IEEE_802_15_4_NETWORK = 23,
 IOTHUBAGENT_NETWORK_BEARER_ETHERNET = 41,
 IOTHUBAGENT_NETWORK_BEARER_DSL = 42,
 IOTHUBAGENT_NETWORK_BEARER_PLC = 43
} iothubAgent_networkBearer;

/**
 * @ingroup dm
 * Callback triggered when DSP request the received signal strength of the device.
 * Network (RXLEV range 0…64) 0 is < -110dBm, 64 is >-48 dBm). Refer to [3GPP 44.018] for more details
 * @return a value between 0 and 64 representing the signal strength. If the return value is greater than 64, it is forced to 64.
 */
typedef uint8_t (*iothubAgent_readRadioSignalStrength)(void);

/**
 * @ingroup init
 * @ingroup dm
 * Structure holding callbacks related to connectivity monitoring object.
 */
typedef struct {
  iothubAgent_readRadioSignalStrength readRadioSignalStrength;  ///< Callback called upon device received signal strength request.
} iothubAgent_connectivityMonitoringCbks;

/**
 * @ingroup dm
 * Set the current network bearer.
 * @param [in] p_networkBearer one of @ref iothubAgent_networkBearer.
 * @return IOTHUBAGENT_OK if the function succeed, one of @ref iothubAgent_Error elsewhere.
 */
iothubAgent_Error iothubAgent_connectivityMonitoring_setNetworkBearer(iothubAgent_networkBearer p_networkBearer);

/**
 * @ingroup dm
 * Set the current APN (access point name) this is relevant only for cellular network.
 * @param [in] p_apn The access point name used by the sim card.
 * @return IOTHUBAGENT_OK if the function succeed, one of @ref iothubAgent_Error elsewhere.
 */
iothubAgent_Error iothubAgent_connectivityMonitoring_setApn(const char* p_apn);

/**
 * @ingroup dm
 * Fill the sim card IMSI info.
 * @param [in] p_imsi The IMSI of the sim card.
 * @return IOTHUBAGENT_OK if the function succeed, one of @ref iothubAgent_Error elsewhere.
 */
iothubAgent_Error iothubAgent_sim_setImsi(const char* p_imsi);

#ifdef __cplusplus
}
#endif

#endif /* PUBLIC_INCLUDE_IOTHUB_AGENT_IOTHUBAGENT_DM_H_ */
