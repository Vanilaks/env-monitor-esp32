#ifndef COMMS_H
#define COMMS_H

/**
 * @brief Initialize communication layer
 * Currently uses UART — swap this file to change protocol
 */
void comms_init(void);

/**
 * @brief Send a message via current communication layer
 * @param message Null-terminated string to send
 */
void comms_send(const char *message);

#endif // COMMS_H