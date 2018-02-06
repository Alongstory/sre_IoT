#ifndef PUBLIC_INCLUDE_IOTHUBAGENT__ERRORCODES_H_
#define PUBLIC_INCLUDE_IOTHUBAGENT__ERRORCODES_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Agent error codes.
 * @ingroup management
 */
typedef enum {IOTHUBAGENT_OK,
              IOTHUBAGENT_KO,                 ///< Unspecified error occurs.
              IOTHUBAGENT_INVALID_ARG,        ///< Provided argument does not match with requirements.
              IOTHUBAGENT_NOT_ENOUGH_MEMORY,  ///< Failed to allocate desired amount of memory.
              IOTHUBAGENT_INVALID_SIZE,       ///< Message size exceed IOTHUBAGENT_MAX_MESSAGE_LENGTH.
              IOTHUBAGENT_DESTROY,            ///< The sent callback is called because the agent is stopping.
              IOTHUBAGENT_TIMEOUT,            ///< Time out reached while waiting for message delivery confirmation.
              IOTHUBAGENT_ABANDON,            ///< Abandon received message
              IOTHUBAGENT_REJECT,             ///< Reject received message
              IOTHUBAGENT_ILLEGAL_STATE       ///< Iot Hub Agent is not in good state to perform operation
} iothubAgent_Error;


#ifdef __cplusplus
}
#endif

#endif /* PUBLIC_INCLUDE_IOTHUBAGENT__ERRORCODES_H_ */
