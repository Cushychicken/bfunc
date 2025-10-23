/**
  ******************************************************************************
  * @file    ad9837_driver.c
  * @brief   AD9837 DDS Driver Implementation
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ad9837_driver.h"

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief Initialize AD9837 DDS chip
 */
HAL_StatusTypeDef AD9837_Init(SPI_HandleTypeDef *hspi, ad9837_dds_ctrl_t *dds_control)
{
    // Initialize control register to known state
    dds_control->reg.freqreg    = 0;
    dds_control->reg.b28        = 1;
    dds_control->reg.hlb        = 0;
    dds_control->reg.fsel       = 0;
    dds_control->reg.psel       = 0;
    dds_control->reg.reserved9  = 0;
    dds_control->reg.reset      = 1;
    dds_control->reg.sleep1     = 0;
    dds_control->reg.sleep12    = 0;
    dds_control->reg.opbiten    = 0;
    dds_control->reg.reserved4  = 0;
    dds_control->reg.div2       = 0;
    dds_control->reg.reserved2  = 0;
    dds_control->reg.mode       = 0;
    dds_control->reg.reserved0  = 0;
    
    return HAL_OK;
}

/**
 * @brief Start DDS output
 */
HAL_StatusTypeDef AD9837_StartOutput(SPI_HandleTypeDef *hspi, ad9837_dds_ctrl_t *dds_control)
{
    dds_control->reg.reset = 0;
    return HAL_SPI_Transmit(hspi, dds_control->data, 1, 10);
}

/**
 * @brief Stop DDS output
 */
HAL_StatusTypeDef AD9837_StopOutput(SPI_HandleTypeDef *hspi, ad9837_dds_ctrl_t *dds_control)
{
    dds_control->reg.reset = 1;
    return HAL_SPI_Transmit(hspi, dds_control->data, 1, 10);
}

/**
 * @brief Set frequency register 0
 */
HAL_StatusTypeDef AD9837_SetFreq0(SPI_HandleTypeDef *hspi, uint32_t frequency)
{
    ad9837_freq_set_t freq0;
    HAL_StatusTypeDef status;
    
    // Set 14 LSBs of Freq0 register
    freq0.reg.freqreg = 1;
    freq0.reg.freqset = (unsigned int)(frequency & 0x00003fff);
    status = HAL_SPI_Transmit(hspi, freq0.data, 1, 10);
    if (status != HAL_OK) return status;
    
    // Set 14 MSBs of Freq0 register
    freq0.reg.freqreg = 1;
    freq0.reg.freqset = (unsigned int)((frequency & 0x0fffc000) >> 14);
    return HAL_SPI_Transmit(hspi, freq0.data, 1, 10);
}

/**
 * @brief Set frequency register 1
 */
HAL_StatusTypeDef AD9837_SetFreq1(SPI_HandleTypeDef *hspi, uint32_t frequency)
{
    ad9837_freq_set_t freq1;
    HAL_StatusTypeDef status;
    
    // Set 14 LSBs of Freq1 register
    freq1.reg.freqreg = 2;
    freq1.reg.freqset = (unsigned int)(frequency & 0x00003fff);
    status = HAL_SPI_Transmit(hspi, freq1.data, 1, 10);
    if (status != HAL_OK) return status;
    
    // Set 14 MSBs of Freq1 register
    freq1.reg.freqreg = 2;
    freq1.reg.freqset = (unsigned int)((frequency & 0x0fffc000) >> 14);
    return HAL_SPI_Transmit(hspi, freq1.data, 1, 10);
}

/**
 * @brief Set phase register 0
 */
HAL_StatusTypeDef AD9837_SetPhase0(SPI_HandleTypeDef *hspi, uint16_t phase)
{
    ad9837_phase_set_t phase0;
    phase0.reg.phasereg = 6; // Must be 0x6 (0b110) to set PHASEREG bit
    phase0.reg.reserved = 0;
    phase0.reg.phaseset = (phase & 0x0fff);
    return HAL_SPI_Transmit(hspi, phase0.data, 1, 10);
}

/**
 * @brief Set phase register 1
 */
HAL_StatusTypeDef AD9837_SetPhase1(SPI_HandleTypeDef *hspi, uint16_t phase)
{
    ad9837_phase_set_t phase1;
    phase1.reg.phasereg = 7; // Must be 0x7 (0b111) to set PHASEREG bit
    phase1.reg.reserved = 0;
    phase1.reg.phaseset = (phase & 0x0fff);
    return HAL_SPI_Transmit(hspi, phase1.data, 1, 10);
}

/**
 * @brief Set waveform mode
 */
HAL_StatusTypeDef AD9837_SetWaveform(SPI_HandleTypeDef *hspi, ad9837_dds_ctrl_t *dds_control, ad9837_waveform_t waveform)
{
    switch (waveform) {
        case AD9837_WAVEFORM_SINE:
            dds_control->reg.opbiten = 0;
            dds_control->reg.mode = 0;
            break;
            
        case AD9837_WAVEFORM_TRIANGLE:
        case AD9837_WAVEFORM_SAWTOOTH:
            dds_control->reg.opbiten = 0;
            dds_control->reg.mode = 1;
            break;
            
        case AD9837_WAVEFORM_SQUARE:
            dds_control->reg.opbiten = 1;
            dds_control->reg.mode = 0;
            dds_control->reg.div2 = 1;
            break;
            
        default:
            return HAL_ERROR;
    }
    
    return HAL_SPI_Transmit(hspi, dds_control->data, 1, 10);
}

/**
 * @brief Toggle frequency register selection
 */
HAL_StatusTypeDef AD9837_ToggleFreqReg(SPI_HandleTypeDef *hspi, ad9837_dds_ctrl_t *dds_control)
{
    dds_control->reg.fsel = (dds_control->reg.fsel == 1) ? 0 : 1;
    return HAL_SPI_Transmit(hspi, dds_control->data, 1, 10);
}

/**
 * @brief Toggle phase register selection
 */
HAL_StatusTypeDef AD9837_TogglePhaseReg(SPI_HandleTypeDef *hspi, ad9837_dds_ctrl_t *dds_control)
{
    static uint8_t phaseout = 0;
    
    if (phaseout == 0) {
        phaseout = 1;
        dds_control->reg.psel = 1;
    } else {
        phaseout = 0;
        dds_control->reg.psel = 0;
    }
    
    return HAL_SPI_Transmit(hspi, dds_control->data, 1, 10);
}
