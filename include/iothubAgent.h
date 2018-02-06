#ifndef PUBLIC_INCLUDE_IOTHUBAGENT_H_
#define PUBLIC_INCLUDE_IOTHUBAGENT_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "iothubAgent_dm.h"
#include "iothubAgent_errorCodes.h"
#include "iothubAgent_logger.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @mainpage
 * @section overview Overview
 *
 * The IoT Hub agent is used to communicate with DSP cloud services.
 * It supports bi-directional messaging over websocket and can be used to perform some device management commands.
 * Internally the agent relies on finite state machine described below.
 *
 * @dot
 *  digraph IoT_Hub_Agent_State_Machine {
 *    splines=ortho;
 *    overlap = scalexy ;
 *    graph [nodesep="2"];
 *    node [shape=record];
 *    IOTHUBAGENT_SHUTDOWN -> IOTHUBAGENT_STOPPED  [headlabel="iothubAgent_init"     URL="@ref iothubAgent_init"     constraint=false];
 *    IOTHUBAGENT_STOPPED ->  IOTHUBAGENT_SHUTDOWN [taillabel="iothubAgent_shutdown" URL="@ref iothubAgent_shutdown" constraint=false];
 *    IOTHUBAGENT_STOPPED ->  IOTHUBAGENT_RUNNING  [headlabel="iothubAgent_start"    URL="@ref iothubAgent_start"    constraint=false];
 *    IOTHUBAGENT_RUNNING ->  IOTHUBAGENT_STOPPED  [taillabel="iothubAgent_stop"     URL="@ref iothubAgent_stop"     constraint=false];
 *    IOTHUBAGENT_RUNNING ->  IOTHUBAGENT_SHUTDOWN [xlabel="iothubAgent_shutdown"    URL="@ref iothubAgent_shutdown" constraint=false];
 *  }
 * @enddot
 *
 * The agent must be provided a connection string before starting.
 * A connection string is retrieved using @ref iothubAgent_bootstrap.
 *
 * Receiving messages from DSP is completely asynchronous and can occurs as soon as the agent is in IOTHUBAGENT_RUNNING state.
 *
 * @defgroup init       Initialize the agent.
 *
 *                      @dot
 *                       digraph IoT_Hub_Agent_State_Machine {
 *                         splines=ortho;
 *                         overlap = scalexy ;
 *                         graph [nodesep="2"];
 *                         node [shape=record];
 *                         IOTHUBAGENT_SHUTDOWN -> IOTHUBAGENT_STOPPED  [headlabel="iothubAgent_init"     URL="@ref iothubAgent_init"     constraint=false];
 *                       }
 *                      @enddot
 *
 *                      This section explains how to initialize the agent.
 *                      Initialization must be done before ant other action.
 *
 * @defgroup boot       Bootstrapping.
 *
 *                      Bootstrapping can be done at any time, after the agent has been initialized.
 *
 *                      @msc
 *                      user_code, agent, bootstrap_service;
 *                      ...;
 *                      user_code =>  agent [label="iothubAgent_bootstrap"];
 *                      agent     =>  bootstrap_service [label="bootstrapRequest"];
 *                      agent     <<  bootstrap_service [label="bootstrapResponse"];
 *                      user_code <<  agent [label="connectionString"];
 *                      ...;
 *                      @endmsc
 *
 * @defgroup start      Starting the agent
 *
 *                      @dot
 *                       digraph IoT_Hub_Agent_State_Machine {
 *                         splines=ortho;
 *                         overlap = scalexy ;
 *                         graph [nodesep="2"];
 *                         node [shape=record];
 *                         IOTHUBAGENT_STOPPED ->  IOTHUBAGENT_RUNNING  [headlabel="iothubAgent_start"    URL="@ref iothubAgent_start"    constraint=false];
 *                       }
 *                      @enddot
 *
 *                      Once you have a connection string, you can start the agent.
 *                      Starting the agent means set the AMQP channel to be able to send and receive messages.
 *                      <b>Even if the start process is successful, this does not mean the device is actually connected to the iot hub</b>
 *
 * @defgroup send       Sending messages.
 *
 *                      @msc
 *                      user_code, agent, hub;
 *                      ...;
 *                      user_code =>  agent [label="iothubAgent_sendMessage(outgoingMessage)"];
 *                      agent     =>  hub;
 *                      ...;
 *                      agent     <<= hub;
 *                      user_code <<= agent [label="iothubAgent_sendConfirmationCbk"];
 *                      ...;
 *                      @endmsc
 *
 * @defgroup receive    Receiving messages.
 *
 *                      Receiving messages from the iot hub is completely asynchronous
 *
 *                      @msc
 *                      user_code, agent, hub;
 *                      ...;
 *                      agent     <<= hub;
 *                      user_code <<= agent [label="iothubAgent_messageReceivedCbk"];
 *                      ...;
 *                      @endmsc
 *
 * @defgroup stop       Stopping the agent.
 *
 *                      @dot
 *                       digraph IoT_Hub_Agent_State_Machine {
 *                         splines=ortho;
 *                         overlap = scalexy ;
 *                         graph [nodesep="2"];
 *                         node [shape=record];
 *                         IOTHUBAGENT_RUNNING ->  IOTHUBAGENT_STOPPED  [taillabel="iothubAgent_stop"     URL="@ref iothubAgent_stop"     constraint=false];
 *                       }
 *                      @enddot
 *
 *                      Stopping the agent means that the agent won't be able to send or receive new messages.
 *                      Messages in stored in the sending queue won't be send, and they will be confirmed (via @ref iothubAgent_sendConfirmationCbk) with the error IOTHUBAGENT_DESTROY
 *
 * @defgroup close      Shutting down the agent
 *
 *                      @dot
 *                       digraph IoT_Hub_Agent_State_Machine {
 *                         splines=ortho;
 *                         overlap = scalexy ;
 *                         graph [nodesep="2"];
 *                         node [shape=record];
 *                         IOTHUBAGENT_RUNNING ->  IOTHUBAGENT_SHUTDOWN [xlabel="iothubAgent_shutdown"    URL="@ref iothubAgent_shutdown" constraint=false];
 *                       }
 *                      @enddot
 *
 * @defgroup dm         Device management related functions.
 *
 *                      Details on DM messages can be found here @ref dm-messages.
 *
 * @defgroup management IoT hub agent management functions.
 *
 *                      These functions are used get and set some useful stuff in the agent.
 *
 * @defgroup logging    Managing the IoT Hub agent logging.
 * */

/**
 * @ingroup send
 * Maximum message length accepted by @ref iothubAgent_sendMessage and @ref iothubAgent_sendMessageFragment
 */
#define IOTHUBAGENT_MAX_MESSAGE_LENGTH 256000

/**
 * @ingroup management
 * iothubAgent states
 */
typedef enum {
  IOTHUBAGENT_SHUTDOWN,   ///< IOTHUBAGENT_SHUTDOWN
  IOTHUBAGENT_STOPPED,    ///< IOTHUBAGENT_STOPPED
  IOTHUBAGENT_RUNNING     ///< IOTHUBAGENT_RUNNING
} iothubAgent_state;

/**
 * @ingroup send
 * @ingroup receive
 * Structure representing the message id
 */
typedef struct  {
	const char* id; ///< Null terminated string representing the id of the message
} iothubAgent_messageId;


typedef struct iothubAgent_messagePropertyNode  iothubAgent_messagePropertyNode;

/**
 * @ingroup send
 * Structure of a linked list containing the message custom properties
 */
struct iothubAgent_messagePropertyNode {
  const char* property; ///< Property key
  const char* value;    ///< Property value
  iothubAgent_messagePropertyNode *next; ///< Pointer to the next @ref iothubAgent_messagePropertyNode
};

/**
 * @ingroup send
 * @ingroup receive
 * Structure representing the message
 */
typedef struct  {
  const unsigned char* payload; ///< Payload of the message, encoding of the payload is unknown for the agent
  size_t size;                  ///< Size of the payload
  iothubAgent_messageId id;     ///< Id of the message, the format is up to the calling code
  iothubAgent_messagePropertyNode* properties; ///< First element of linked list containing the message custom properties
} iothubAgent_message;

/**
 * @ingroup init
 * Structure containing all the http proxy information
 */
typedef struct {
  char* host_address; ///< http proxy host name (set to NULL to disable proxy)
  int port;           ///< http proxy port
  char* username;     ///< basic authentication username (not required)
  char* password;     ///< basic authentication password (not required)
} iothubAgent_httpProxyConfiguration;

/**
 * @ingroup init
 * Callback triggered to confirm a device to cloud event is handled by the DSP service.
 * All data corresponding to the specified message are not needed any more.
 * @param [in] p_messageId The message id
 * @param [in] p_err The reason for calling see @ref iothubAgent_Error for possible errors.
 * @return void
 */
typedef void(*iothubAgent_sendConfirmationCbk)(iothubAgent_messageId p_messageId, iothubAgent_Error p_err);

/**
 * @ingroup receive
 * Call back called upon message reception.
 * @param [in] The message received
 * @return A @ref iothubAgent_Error, see @ref iothubAgent_Error for possible usable errors.
 */
typedef iothubAgent_Error(*iothubAgent_messageReceivedCbk)(iothubAgent_message* p_message);

/**
 * @ingroup init
 * Call back called when agent state has changed
 * @param [in] The new agent state
 */
typedef void(*iothubAgent_onStateChangedCbk)(iothubAgent_state  p_state);

/**
 * @ingroup init
 * Structure representing the configuration of the agent
 */
typedef struct {
  iothubAgent_sendConfirmationCbk sendConfirmationCbk;     ///< Callback triggered when send confirmation is received
  iothubAgent_messageReceivedCbk messageReceivedCbk;       ///< Callback triggered when a message is received
  iothubAgent_deviceCbks deviceCbks;                       ///< Structure containing all the callback dealing with LWM2M device object
  iothubAgent_firmwareUpdateCbks firmwareUpdateCbks;       ///< Structure containing all the callback dealing with LWM2M firmware object
  iothubAgent_connectivityMonitoringCbks connectivityMonitoringCbks; ////< Structure containing all the callback dealing with LWM2M network monitoring object.
  iothubAgent_onStateChangedCbk onStateChangedCbk;        ///< Callback triggered when agent state has changed
} iothubAgent_configuration;

/**
 * @ingroup init
 * Initialize the agent with a given iothubAgent_configuration
 * @param[in] p_configuration The @ref iothubAgent_configuration to use.
 * @return IOTHUBAGENT_OK if the function succeed otherwise an @ref iothubAgent_Error code.
 */
iothubAgent_Error iothubAgent_init(const iothubAgent_configuration* p_configuration);

/**
 * @ingroup boot
 * Credentials to be used for bootstrapping, all fields are required.
 */
typedef struct  {
  const char* bootstrapServiceURL;  ///< The url of the boot strap service root to use. i.e https://bootstrap.struxurewarecloud.com, be sure to not add a final / in the url.
  const char* deviceEndpointId;     ///< Id of the device in iot hub
  const char* bootstrapKey;         ///< Key for boot strap service connection
} iothubAgent_bootstrapServiceCredentials;

/**
 * @ingroup boot
 * Sends a bootstrap request to the bootstrap service. Available only in @ref IOTHUBAGENT_STOPPED state
 * @param[in] p_credentials Credentials to use to connect to the boost strap service.
 * @param[out] p_connectionString A pointer to a pointer where the connection string will be stored.
 * @return IOTHUBAGENT_OK if the function succeed otherwise a @ref iothubAgent_Error code.
 */
iothubAgent_Error iothubAgent_bootstrap(const iothubAgent_bootstrapServiceCredentials* p_credentials, char** p_connectionString);

/**
 * @ingroup start
 * Start a configured agent.
 * @param[in] p_connectionString the connection provided by @ref iothubAgent_bootstrap to connect to IoT Hub
 * @return IOTHUBAGENT_OK if the function succeed otherwise a @ref iothubAgent_Error code.
 */
iothubAgent_Error iothubAgent_start(const char* p_connectionString);

/**
 * @ingroup send
 * Send a complete (non fragmented) message to iotHub.
 * @param[in] p_message The message to send
 * @return IOTHUBAGENT_OK if the function succeed otherwise a @ref iothubAgent_Error code.
 */
iothubAgent_Error iothubAgent_sendMessage(const iothubAgent_message* p_message);

/**
 * @ingroup send
 * Send a message fragment to iotHub.
 * @param[in] p_message The message to send.
 * @param[in] p_fragmentNumber The fragment number of the message.
 * @param[in] p_isFinalMessage Flag to set if the current fragment is the last one.
 * @return IOTHUBAGENT_OK if the function succeed otherwise a @ref iothubAgent_Error code.
 */
iothubAgent_Error iothubAgent_sendMessageFragment(const iothubAgent_message* p_message, unsigned long p_fragmentNumber, bool p_isFinalMessage);

/**
 * @ingroup stop
 * Stop the agent, calling @ref iothubAgent_start is possible after a call to @ref iothubAgent_stop.
 * @return IOTHUBAGENT_OK
 */
iothubAgent_Error iothubAgent_stop(void);

/**
 * @ingroup close
 * Uninitialized all internal stuff. This call blocks until all resources are freed.
 * @return IOTHUBAGENT_OK if the function succeed otherwise a @ref iothubAgent_Error code.
 */
iothubAgent_Error iothubAgent_shutdown(void);

/**
 * @ingroup management
 * Get the state of the agent.
 * @return One of iothubAgent_state
 */
iothubAgent_state iothubAgent_getState(void);

/**
 * @ingroup management
 * Returns a string containing the state name.
 * @param p_state The state to get the name of.
 * @return Pointer to a null terminated string containing the state name
 */
const char* iothubAgent_getStateName(iothubAgent_state p_state);

/**
 * @ingroup management
 * Get the version of the IoT Hub agent.
 * @return  Pointer to a null terminated string containing the current IoT Hub Agent library version.
 */
 const char* iothubAgent_getVersion(void);

 /**
  * @ingroup management
  * Set at run time the iothubAgent logger level
  * @param [in] p_level the log level to set
  */
 void iothubAgent_setLoggerLevel(iothubAgent_loggerLevel_t p_level);

 /**
  * @ingroup management
  * Set the proxy to use for the next start and bootstrap request. This function works only in @ref IOTHUBAGENT_STOPPED state
  * @param [in] p_httpProxyConfiguration structure containing proxy http configuration, set field host_address to NULL to disable proxy
  * <b>no scheme</b>. All the strings are internally copied, so deletion of any field of structure is safe after function call;
  * @return IOTHUBAGENT_OK if the function succeed otherwise a @ref iothubAgent_Error code.
  */
 iothubAgent_Error iothubAgent_setProxy(iothubAgent_httpProxyConfiguration p_httpProxyConfiguration);

#ifdef __cplusplus
}
#endif

#endif /* PUBLIC_INCLUDE_IOTHUBAGENT_H_ */
