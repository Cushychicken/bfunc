/**
  ******************************************************************************
  * @file    waveform_controller.c
  * @brief   Waveform Controller Implementation
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "waveform_controller.h"

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static HAL_StatusTypeDef SetSymbolRateTimer(TIM_TypeDef *TIMx, uint16_t ms);
static void StopModulationTimer(TIM_TypeDef *TIMx);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief Initialize waveform controller
 */
HAL_StatusTypeDef WaveformController_Init(waveform_controller_t *controller, 
                                         SPI_HandleTypeDef *hspi, 
                                         TIM_HandleTypeDef *htim)
{
    controller->hspi = hspi;
    controller->htim = htim;
    controller->current_state = WAVEFORM_IDLE;
    controller->freq0_value = 0;
    controller->freq1_value = 0;
    controller->phase0_value = 0;
    controller->phase1_value = 0;
    
    // Initialize AD9837
    HAL_StatusTypeDef status = AD9837_Init(hspi, &controller->dds_control);
    if (status != HAL_OK) return status;
    
    // Stop output initially
    return AD9837_StopOutput(hspi, &controller->dds_control);
}

/**
 * @brief Set waveform state
 */
HAL_StatusTypeDef WaveformController_SetState(waveform_controller_t *controller, 
                                             waveform_state_t state)
{
    HAL_StatusTypeDef status = HAL_OK;
    
    controller->current_state = state;
    
    switch (state) {
        case WAVEFORM_SINE:
            status = AD9837_SetWaveform(controller->hspi, &controller->dds_control, AD9837_WAVEFORM_SINE);
            if (status == HAL_OK) {
                status = AD9837_StartOutput(controller->hspi, &controller->dds_control);
            }
            break;
            
        case WAVEFORM_TRIANGLE:
            status = AD9837_SetWaveform(controller->hspi, &controller->dds_control, AD9837_WAVEFORM_TRIANGLE);
            if (status == HAL_OK) {
                status = AD9837_StartOutput(controller->hspi, &controller->dds_control);
            }
            break;
            
        case WAVEFORM_SQUARE:
            status = AD9837_SetWaveform(controller->hspi, &controller->dds_control, AD9837_WAVEFORM_SQUARE);
            if (status == HAL_OK) {
                status = AD9837_StartOutput(controller->hspi, &controller->dds_control);
            }
            break;
            
        case WAVEFORM_SAWTOOTH:
            status = AD9837_SetWaveform(controller->hspi, &controller->dds_control, AD9837_WAVEFORM_SAWTOOTH);
            if (status == HAL_OK) {
                status = AD9837_StartOutput(controller->hspi, &controller->dds_control);
            }
            break;
            
        case WAVEFORM_MOD_PSK:
            status = AD9837_SetWaveform(controller->hspi, &controller->dds_control, AD9837_WAVEFORM_SINE);
            if (status == HAL_OK) {
                status = AD9837_StartOutput(controller->hspi, &controller->dds_control);
            }
            break;
            
        case WAVEFORM_IDLE:
        default:
            status = AD9837_StopOutput(controller->hspi, &controller->dds_control);
            StopModulationTimer(controller->htim->Instance);
            break;
    }
    
    return status;
}

/**
 * @brief Set frequency 0
 */
HAL_StatusTypeDef WaveformController_SetFreq0(waveform_controller_t *controller, 
                                             uint32_t frequency)
{
    controller->freq0_value = (uint32_t)(FREQ_CONVERSION_FACTOR * (float)frequency);
    return AD9837_SetFreq0(controller->hspi, controller->freq0_value);
}

/**
 * @brief Set frequency 1
 */
HAL_StatusTypeDef WaveformController_SetFreq1(waveform_controller_t *controller, 
                                             uint32_t frequency)
{
    controller->freq1_value = (uint32_t)(FREQ_CONVERSION_FACTOR * (float)frequency);
    return AD9837_SetFreq1(controller->hspi, controller->freq1_value);
}

/**
 * @brief Set phase 0
 */
HAL_StatusTypeDef WaveformController_SetPhase0(waveform_controller_t *controller, 
                                              uint16_t phase)
{
    controller->phase0_value = (uint16_t)(PHASE_CONVERSION_FACTOR * (float)phase);
    return AD9837_SetPhase0(controller->hspi, controller->phase0_value);
}

/**
 * @brief Set phase 1
 */
HAL_StatusTypeDef WaveformController_SetPhase1(waveform_controller_t *controller, 
                                              uint16_t phase)
{
    controller->phase1_value = (uint16_t)(PHASE_CONVERSION_FACTOR * (float)phase);
    return AD9837_SetPhase1(controller->hspi, controller->phase1_value);
}

/**
 * @brief Toggle frequency register
 */
HAL_StatusTypeDef WaveformController_ToggleFreqReg(waveform_controller_t *controller)
{
    return AD9837_ToggleFreqReg(controller->hspi, &controller->dds_control);
}

/**
 * @brief Toggle phase register
 */
HAL_StatusTypeDef WaveformController_TogglePhaseReg(waveform_controller_t *controller)
{
    return AD9837_TogglePhaseReg(controller->hspi, &controller->dds_control);
}

/**
 * @brief Set symbol rate for modulation
 */
HAL_StatusTypeDef WaveformController_SetSymbolRate(waveform_controller_t *controller, 
                                                   uint16_t ms)
{
    return SetSymbolRateTimer(controller->htim->Instance, ms);
}

/**
 * @brief Stop modulation
 */
HAL_StatusTypeDef WaveformController_StopModulation(waveform_controller_t *controller)
{
    StopModulationTimer(controller->htim->Instance);
    return HAL_OK;
}

/**
 * @brief Get current state
 */
waveform_state_t WaveformController_GetState(waveform_controller_t *controller)
{
    return controller->current_state;
}

/**
 * @brief Set symbol rate timer
 */
static HAL_StatusTypeDef SetSymbolRateTimer(TIM_TypeDef *TIMx, uint16_t ms)
{
    TIMx->CR1 &= ~(TIM_CR1_CEN);
    
    if (TIMx == TIM2) {
        RCC->APB1RSTR |= (RCC_APB1RSTR_TIM2RST);
        RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM2RST);
    }
    
    TIMx->PSC = 48000000/1000;
    TIMx->ARR = ms;
    
    TIMx->EGR  |= TIM_EGR_UG; 
    TIMx->DIER |= TIM_DIER_UIE; 
    TIMx->CR1  |= TIM_CR1_CEN;
    
    return HAL_OK;
}

/**
 * @brief Stop modulation timer
 */
static void StopModulationTimer(TIM_TypeDef *TIMx)
{
    TIMx->CR1 &= ~(TIM_CR1_CEN);
}
