/**
  ******************************************************************************
  * @file    command_parser.h
  * @brief   Command Parser Header
  ******************************************************************************
  */

#ifndef __COMMAND_PARSER_H
#define __COMMAND_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "waveform_controller.h"

/* Exported types ------------------------------------------------------------*/
typedef struct {
    uint8_t buffer[64];
    uint8_t index;
} command_buffer_t;

/* Exported constants --------------------------------------------------------*/
#define MAX_COMMAND_LENGTH 64
#define MAX_PARAMETERS 10

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Initialize command buffer
 * @param buffer Command buffer structure
 * @retval None
 */
void CommandParser_InitBuffer(command_buffer_t *buffer);

/**
 * @brief Add character to command buffer
 * @param buffer Command buffer structure
 * @param c Character to add
 * @retval 1 if command is complete, 0 otherwise
 */
uint8_t CommandParser_AddChar(command_buffer_t *buffer, char c);

/**
 * @brief Process complete command
 * @param buffer Command buffer structure
 * @param controller Waveform controller
 * @retval None
 */
void CommandParser_ProcessCommand(command_buffer_t *buffer, waveform_controller_t *controller);

/**
 * @brief Clear command buffer
 * @param buffer Command buffer structure
 * @retval None
 */
void CommandParser_ClearBuffer(command_buffer_t *buffer);

#ifdef __cplusplus
}
#endif

#endif /* __COMMAND_PARSER_H */
