/**
  ******************************************************************************
  * @file    waveform_controller.h
  * @brief   Waveform Controller Header
  ******************************************************************************
  */

#ifndef __WAVEFORM_CONTROLLER_H
#define __WAVEFORM_CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "ad9837_driver.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
    WAVEFORM_IDLE = 0,
    WAVEFORM_SINE,
    WAVEFORM_TRIANGLE,
    WAVEFORM_SQUARE,
    WAVEFORM_SAWTOOTH,
    WAVEFORM_MOD_PSK
} waveform_state_t;

typedef struct {
    SPI_HandleTypeDef *hspi;
    TIM_HandleTypeDef *htim;
    ad9837_dds_ctrl_t dds_control;
    waveform_state_t current_state;
    uint32_t freq0_value;
    uint32_t freq1_value;
    uint16_t phase0_value;
    uint16_t phase1_value;
} waveform_controller_t;

/* Exported constants --------------------------------------------------------*/
#define FREQ_CONVERSION_FACTOR 16.777216f
#define PHASE_CONVERSION_FACTOR 11.377774f

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Initialize waveform controller
 * @param controller Waveform controller structure
 * @param hspi SPI handle
 * @param htim Timer handle
 * @retval HAL status
 */
HAL_StatusTypeDef WaveformController_Init(waveform_controller_t *controller, 
                                         SPI_HandleTypeDef *hspi, 
                                         TIM_HandleTypeDef *htim);

/**
 * @brief Set waveform state
 * @param controller Waveform controller structure
 * @param state New waveform state
 * @retval HAL status
 */
HAL_StatusTypeDef WaveformController_SetState(waveform_controller_t *controller, 
                                             waveform_state_t state);

/**
 * @brief Set frequency 0
 * @param controller Waveform controller structure
 * @param frequency Frequency in Hz
 * @retval HAL status
 */
HAL_StatusTypeDef WaveformController_SetFreq0(waveform_controller_t *controller, 
                                             uint32_t frequency);

/**
 * @brief Set frequency 1
 * @param controller Waveform controller structure
 * @param frequency Frequency in Hz
 * @retval HAL status
 */
HAL_StatusTypeDef WaveformController_SetFreq1(waveform_controller_t *controller, 
                                             uint32_t frequency);

/**
 * @brief Set phase 0
 * @param controller Waveform controller structure
 * @param phase Phase in degrees
 * @retval HAL status
 */
HAL_StatusTypeDef WaveformController_SetPhase0(waveform_controller_t *controller, 
                                              uint16_t phase);

/**
 * @brief Set phase 1
 * @param controller Waveform controller structure
 * @param phase Phase in degrees
 * @retval HAL status
 */
HAL_StatusTypeDef WaveformController_SetPhase1(waveform_controller_t *controller, 
                                              uint16_t phase);

/**
 * @brief Toggle frequency register
 * @param controller Waveform controller structure
 * @retval HAL status
 */
HAL_StatusTypeDef WaveformController_ToggleFreqReg(waveform_controller_t *controller);

/**
 * @brief Toggle phase register
 * @param controller Waveform controller structure
 * @retval HAL status
 */
HAL_StatusTypeDef WaveformController_TogglePhaseReg(waveform_controller_t *controller);

/**
 * @brief Set symbol rate for modulation
 * @param controller Waveform controller structure
 * @param ms Symbol rate in milliseconds
 * @retval HAL status
 */
HAL_StatusTypeDef WaveformController_SetSymbolRate(waveform_controller_t *controller, 
                                                   uint16_t ms);

/**
 * @brief Stop modulation
 * @param controller Waveform controller structure
 * @retval HAL status
 */
HAL_StatusTypeDef WaveformController_StopModulation(waveform_controller_t *controller);

/**
 * @brief Get current state
 * @param controller Waveform controller structure
 * @retval Current waveform state
 */
waveform_state_t WaveformController_GetState(waveform_controller_t *controller);

#ifdef __cplusplus
}
#endif

#endif /* __WAVEFORM_CONTROLLER_H */
