/**
  ******************************************************************************
  * @file    command_parser.c
  * @brief   Command Parser Implementation
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "command_parser.h"
#include "usbd_cdc_if.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef BUILD_ID_AVAILABLE
typedef struct {
	uint32_t namesz;
	uint32_t descsz;
	uint32_t type;
	uint8_t data[];	
} ElfNoteSection_t;

extern const ElfNoteSection_t g_note_build_id;
#endif

/* Private function prototypes -----------------------------------------------*/
static void SendResponse(const char *str);
static void SendBuildId(void);
static void SendPrompt(void);
static void ParseCommand(const char *cmd, waveform_controller_t *controller);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief Initialize command buffer
 */
void CommandParser_InitBuffer(command_buffer_t *buffer)
{
    memset(buffer->buffer, 0, sizeof(buffer->buffer));
    buffer->index = 0;
}

/**
 * @brief Add character to command buffer
 */
uint8_t CommandParser_AddChar(command_buffer_t *buffer, char c)
{
    switch (c) {
        case '\b':  // Backspace
            if (buffer->index > 0) {
                buffer->buffer[--buffer->index] = '\0';
                SendResponse(" \b");
            }
            break;
            
        case 0x7f:  // Backspace on Mac
            if (buffer->index > 0) {
                buffer->buffer[--buffer->index] = '\0';
                SendResponse("\b \b");
            }
            break;
            
        case '\n':  // Newline
        case '\r':  // Carriage return
            if (buffer->index > 0) {
                ParseCommand((char*)buffer->buffer, NULL); // Will be set by caller
                CommandParser_ClearBuffer(buffer);
                return 1; // Command complete
            }
            break;
            
        default:    // Any other character
            if (buffer->index < MAX_COMMAND_LENGTH - 1) {
                buffer->buffer[buffer->index++] = c;
                buffer->buffer[buffer->index] = '\0';
            }
            break;
    }
    
    return 0; // Command not complete
}

/**
 * @brief Process complete command
 */
void CommandParser_ProcessCommand(command_buffer_t *buffer, waveform_controller_t *controller)
{
    ParseCommand((char*)buffer->buffer, controller);
    CommandParser_ClearBuffer(buffer);
}

/**
 * @brief Clear command buffer
 */
void CommandParser_ClearBuffer(command_buffer_t *buffer)
{
    memset(buffer->buffer, 0, sizeof(buffer->buffer));
    buffer->index = 0;
}

/**
 * @brief Send response string
 */
static void SendResponse(const char *str)
{
    uint8_t result = CDC_Transmit_FS((uint8_t*)str, strlen(str));
    while (result == USBD_BUSY) {
        result = CDC_Transmit_FS((uint8_t*)str, strlen(str));
    }
}

/**
 * @brief Send build ID
 */
static void SendBuildId(void)
{
#ifdef BUILD_ID_AVAILABLE
    const uint8_t *build_id_data = &g_note_build_id.data[g_note_build_id.namesz];
    
    SendResponse("Build ID: ");
    
    for (int i = 0; i < g_note_build_id.descsz; ++i) {
        char buildchar[3];
        sprintf(buildchar, "%02x", build_id_data[i]);
        SendResponse(buildchar);
    }
    
    SendResponse("\n\r");
#else
    SendResponse("Build ID: Not available\n\r");
#endif
}

/**
 * @brief Send command prompt
 */
static void SendPrompt(void)
{
    SendResponse("$> ");
}

/**
 * @brief Parse and execute command
 */
static void ParseCommand(const char *cmd, waveform_controller_t *controller)
{
    char delim = ' ';
    char *parms[MAX_PARAMETERS];
    int position = 0;
    
    // Split command into parameters
    char *cmd_copy = malloc(strlen(cmd) + 1);
    strcpy(cmd_copy, cmd);
    
    char *ptr_split = strtok(cmd_copy, &delim);
    while (ptr_split != NULL && position < MAX_PARAMETERS - 1) {
        parms[position] = ptr_split;
        ptr_split = strtok(NULL, &delim);
        position++;
    }
    parms[position] = NULL;
    
    if (position == 0) {
        free(cmd_copy);
        SendPrompt();
        return;
    }
    
    // Send newline
    SendResponse("\n\r");
    
    // Echo parameters
    for (int i = 0; i < position; i++) {
        SendResponse(parms[i]);
        SendResponse("\n\r");
    }
    
    // Process command
    if (strcmp(parms[0], "sine") == 0) {
        if (position > 1) {
            uint32_t freq = atoi(parms[1]);
            WaveformController_SetFreq0(controller, freq);
        }
        if (position > 2) {
            uint16_t phase = atoi(parms[2]);
            WaveformController_SetPhase0(controller, phase);
        }
        WaveformController_SetState(controller, WAVEFORM_SINE);
    }
    else if (strcmp(parms[0], "triangle") == 0) {
        if (position > 1) {
            uint32_t freq = atoi(parms[1]);
            WaveformController_SetFreq0(controller, freq);
        }
        if (position > 2) {
            uint16_t phase = atoi(parms[2]);
            WaveformController_SetPhase0(controller, phase);
        }
        WaveformController_SetState(controller, WAVEFORM_TRIANGLE);
    }
    else if (strcmp(parms[0], "square") == 0) {
        if (position > 1) {
            uint32_t freq = atoi(parms[1]);
            WaveformController_SetFreq0(controller, freq);
        }
        if (position > 2) {
            uint16_t phase = atoi(parms[2]);
            WaveformController_SetPhase0(controller, phase);
        }
        WaveformController_SetState(controller, WAVEFORM_SQUARE);
    }
    else if (strcmp(parms[0], "sawtooth") == 0) {
        if (position > 1) {
            uint32_t freq = atoi(parms[1]);
            WaveformController_SetFreq0(controller, freq);
            WaveformController_SetFreq1(controller, freq);
        }
        if (position > 2) {
            uint16_t phase = atoi(parms[2]);
            WaveformController_SetPhase0(controller, phase + 270);
            WaveformController_SetPhase1(controller, phase + 90);
        } else {
            WaveformController_SetPhase0(controller, 270);
            WaveformController_SetPhase1(controller, 90);
        }
        WaveformController_SetState(controller, WAVEFORM_SAWTOOTH);
    }
    else if (strcmp(parms[0], "freq0") == 0) {
        if (position > 1) {
            uint32_t freq = atoi(parms[1]);
            WaveformController_SetFreq0(controller, freq);
        }
    }
    else if (strcmp(parms[0], "freq1") == 0) {
        if (position > 1) {
            uint32_t freq = atoi(parms[1]);
            WaveformController_SetFreq1(controller, freq);
        }
    }
    else if (strcmp(parms[0], "phase0") == 0) {
        if (position > 1) {
            uint16_t phase = atoi(parms[1]);
            WaveformController_SetPhase0(controller, phase);
        }
    }
    else if (strcmp(parms[0], "phase1") == 0) {
        if (position > 1) {
            uint16_t phase = atoi(parms[1]);
            WaveformController_SetPhase1(controller, phase);
        }
    }
    else if (strcmp(parms[0], "freqsel") == 0) {
        WaveformController_ToggleFreqReg(controller);
    }
    else if (strcmp(parms[0], "phasesel") == 0) {
        WaveformController_TogglePhaseReg(controller);
    }
    else if (strcmp(parms[0], "mod_bpsk") == 0) {
        if (position > 1) {
            uint32_t freq = atoi(parms[1]);
            WaveformController_SetFreq0(controller, freq);
            WaveformController_SetFreq1(controller, freq);
        }
        WaveformController_SetPhase0(controller, 0);
        WaveformController_SetPhase1(controller, 180);
        if (position > 2) {
            uint16_t symbol_rate = atoi(parms[2]);
            WaveformController_SetSymbolRate(controller, symbol_rate);
        }
        WaveformController_SetState(controller, WAVEFORM_MOD_PSK);
    }
    else if (strcmp(parms[0], "idle") == 0) {
        WaveformController_SetState(controller, WAVEFORM_IDLE);
    }
    else if (strcmp(parms[0], "buildid") == 0) {
        SendBuildId();
    }
    else if (strcmp(parms[0], "version") == 0) {
        SendResponse("Firmware Version: ");
        SendResponse(GIT_VERSION);
        SendResponse("\n\r");
    }
    else if (strcmp(parms[0], "upgrade") == 0) {
        // DFU bootloader entry - this would need to be implemented
        SendResponse("DFU mode not implemented\n\r");
    }
    
    free(cmd_copy);
    SendPrompt();
}
