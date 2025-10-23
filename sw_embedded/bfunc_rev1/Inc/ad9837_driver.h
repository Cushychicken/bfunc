/**
  ******************************************************************************
  * @file    ad9837_driver.h
  * @brief   AD9837 DDS Driver Header
  ******************************************************************************
  */

#ifndef __AD9837_DRIVER_H
#define __AD9837_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Exported types ------------------------------------------------------------*/
typedef struct {
    unsigned int reserved0  : 1;
    unsigned int mode       : 1;
    unsigned int reserved2  : 1;
    unsigned int div2       : 1;
    unsigned int reserved4  : 1;
    unsigned int opbiten    : 1;
    unsigned int sleep12    : 1;
    unsigned int sleep1     : 1;
    unsigned int reset      : 1;
    unsigned int reserved9  : 1;
    unsigned int psel       : 1;
    unsigned int fsel       : 1;
    unsigned int hlb        : 1;
    unsigned int b28        : 1;
    unsigned int freqreg    : 2;
} ad9837_ctrl_reg_t;

typedef struct {
    unsigned int freqset    : 14;
    unsigned int freqreg    : 2;
} ad9837_freq_reg_t;

typedef struct {
    unsigned int phaseset   : 12;
    unsigned int reserved   : 1;
    unsigned int phasereg   : 3;
} ad9837_phase_reg_t;

typedef union {
    ad9837_ctrl_reg_t reg;
    uint8_t data[2];
} ad9837_dds_ctrl_t;

typedef union {
    ad9837_freq_reg_t reg;
    uint8_t data[2];
} ad9837_freq_set_t;

typedef union {
    ad9837_phase_reg_t reg;
    uint8_t data[2];
} ad9837_phase_set_t;

typedef enum {
    AD9837_WAVEFORM_SINE = 0,
    AD9837_WAVEFORM_TRIANGLE,
    AD9837_WAVEFORM_SQUARE,
    AD9837_WAVEFORM_SAWTOOTH
} ad9837_waveform_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief Initialize AD9837 DDS chip
 * @param hspi SPI handle
 * @param dds_control Control register structure
 * @retval HAL status
 */
HAL_StatusTypeDef AD9837_Init(SPI_HandleTypeDef *hspi, ad9837_dds_ctrl_t *dds_control);

/**
 * @brief Start DDS output
 * @param hspi SPI handle
 * @param dds_control Control register structure
 * @retval HAL status
 */
HAL_StatusTypeDef AD9837_StartOutput(SPI_HandleTypeDef *hspi, ad9837_dds_ctrl_t *dds_control);

/**
 * @brief Stop DDS output
 * @param hspi SPI handle
 * @param dds_control Control register structure
 * @retval HAL status
 */
HAL_StatusTypeDef AD9837_StopOutput(SPI_HandleTypeDef *hspi, ad9837_dds_ctrl_t *dds_control);

/**
 * @brief Set frequency register 0
 * @param hspi SPI handle
 * @param frequency Frequency value
 * @retval HAL status
 */
HAL_StatusTypeDef AD9837_SetFreq0(SPI_HandleTypeDef *hspi, uint32_t frequency);

/**
 * @brief Set frequency register 1
 * @param hspi SPI handle
 * @param frequency Frequency value
 * @retval HAL status
 */
HAL_StatusTypeDef AD9837_SetFreq1(SPI_HandleTypeDef *hspi, uint32_t frequency);

/**
 * @brief Set phase register 0
 * @param hspi SPI handle
 * @param phase Phase value
 * @retval HAL status
 */
HAL_StatusTypeDef AD9837_SetPhase0(SPI_HandleTypeDef *hspi, uint16_t phase);

/**
 * @brief Set phase register 1
 * @param hspi SPI handle
 * @param phase Phase value
 * @retval HAL status
 */
HAL_StatusTypeDef AD9837_SetPhase1(SPI_HandleTypeDef *hspi, uint16_t phase);

/**
 * @brief Set waveform mode
 * @param hspi SPI handle
 * @param dds_control Control register structure
 * @param waveform Waveform type
 * @retval HAL status
 */
HAL_StatusTypeDef AD9837_SetWaveform(SPI_HandleTypeDef *hspi, ad9837_dds_ctrl_t *dds_control, ad9837_waveform_t waveform);

/**
 * @brief Toggle frequency register selection
 * @param hspi SPI handle
 * @param dds_control Control register structure
 * @retval HAL status
 */
HAL_StatusTypeDef AD9837_ToggleFreqReg(SPI_HandleTypeDef *hspi, ad9837_dds_ctrl_t *dds_control);

/**
 * @brief Toggle phase register selection
 * @param hspi SPI handle
 * @param dds_control Control register structure
 * @retval HAL status
 */
HAL_StatusTypeDef AD9837_TogglePhaseReg(SPI_HandleTypeDef *hspi, ad9837_dds_ctrl_t *dds_control);

#ifdef __cplusplus
}
#endif

#endif /* __AD9837_DRIVER_H */
