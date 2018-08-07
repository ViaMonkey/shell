/***************************************************************************//**
* @file
* @brief C File vt100.c
* @details Source code for TODOdescription
*
* @author Auban le Grelle
*
* @date 7 avr. 2014 21:46:02
*
* <B>Contact:</B> a.legrelle@lgelectronicsystems.com
*
* @copyright (c) 2012, Electronic Systems
*
* <B>Distribution:</B> This file is part of EmbeddedLib.
*
*    EmbeddedLib is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    EmbeddedLib is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General Public License
*    along with EmbeddedLib.  If not, see <http://www.gnu.org/licenses/>.
*
*******************************************************************************/


#include <stdio.h>

#include "vt100.h"


/*** Escape Sequence ***/
#define VT_ESC_SEQ          "\033["
#define VT_SPECIAL_CHAR_CMD '['
#define VT_SPECIAL_CHAR_G0  '('
#define VT_SPECIAL_CHAR_G1  ')'
#define VT_SPECIAL_KEY      '~'


/******************************* VT Command ***********************************/
#define VT_CMD_RESET        'c'             //!< Reset Device
#define VT_CMD_DECSC        '7'             //!< Save Cursor (DEC Private)
#define VT_CMD_DECRC        '8'             //!< Restore Cursor (DEC Private)


/*** Cursor handling ***/
#define VT_CMD_CURSOR               'H'     //!< Home-positioning
#define VT_CMD_CURSOR_SAVE          's'     //!< Save current cursor position
#define VT_CMD_CURSOR_RESTORE       'u'     //!< Restore current cursor position


/*** Scrolling Region ***/
#define VT_CMD_SCROLL               'r'     //!< Set Scrolling Region


/*** Erasing text ***/
#define VT_CMD_ERASE_LINE           'K'
#define VT_CMD_ERASE_SCREEN         'J'
#define VT_CMD_ERASE_TAB            'g'     //!< Tab Clear


/*** Text Mode Attributes ***/
#define VT_CMD_ATTR                 'm'

/*** Text cursor position ***/
#define VT_CMD_CURSOR_POS           'R'







//Declare Private Prototype
static int32_t vt_esc_char_process(vt100_t *pvt, int32_t ch);
static int32_t vt_esc_special_process(vt100_t *pvt, int32_t ch);





//Private function
static int32_t vt_esc_char_process(vt100_t *pvt, int32_t ch)
{
  switch (ch) {
    case VT_SPECIAL_CHAR_G0:                    // '('
        pvt->esc_char = VT_SPECIAL_CHAR_G0;
        return EOF;
    case VT_SPECIAL_CHAR_G1:                    // ')'
        pvt->esc_char = VT_SPECIAL_CHAR_G1;
        return EOF;
    case VT_SPECIAL_CHAR_CMD:                        // '['
        pvt->esc_cur_num = 0;
        pvt->esc_read_num = 0;
        pvt->esc_elems = 0;
        pvt->esc_char = VT_SPECIAL_CHAR_CMD;
        return EOF;
    case VT_CMD_DECSC:                          //DECSC � Save Cursor (DEC Private)
        break;
    case VT_CMD_DECRC:                          //DECRC � Restore Cursor (DEC Private)
        break;
    default:
        ch = 0;
        *pvt->out_buffer = KEY_BEL;
        *(pvt->out_buffer + 1) = ch;

  }



  pvt->esc_char = 0;

  return ch;
}


static int32_t vt_esc_special_process(vt100_t *pvt, int32_t ch)
{
    if ((ch >= '0') && (ch <= '9')) {
        pvt->esc_cur_num = (pvt->esc_cur_num * 10) + (ch - '0');
        pvt->esc_read_num = true;
        return EOF;
    }

    if ((pvt->esc_read_num) || (ch == ';')) {
        if (pvt->esc_elems < VT_ESC_ELEM_SIZE) {
            pvt->esc_elem[pvt->esc_elems++] = pvt->esc_cur_num;
        }
        pvt->esc_read_num = false;
    }

    pvt->esc_cur_num = 0;

    switch (ch) {
        case '?':
        case ';':
            return EOF;
        case VT_MOVE_CUR_LEFT:
            ch = KB_LEFT;
            break;
        case VT_MOVE_CUR_RIGHT:
            ch = KB_RIGHT;
            break;
        case VT_MOVE_CUR_UP:
            ch = KB_UP;
            break;
        case VT_MOVE_CUR_DOWN:
            ch = KB_DOWN;
            break;
        case VT_CMD_CURSOR_POS:
            pvt->row_pos = pvt->esc_elem[0];
            pvt->col_pos = pvt->esc_elem[1];
            ch = 0;
            break;
        default:
            ch = 0;
            *pvt->out_buffer = KEY_BEL;
            *(pvt->out_buffer + 1) = ch;
    }

    pvt->esc_char = 0;

    return ch;
}





//Public Function
//*****************************************************************************
//return -1 = EOF
int32_t vtProcessChar(vt100_t *pvt, int32_t ch)
{

    if (pvt->is_esc == true) {
        switch (pvt->esc_char) {
            case VT_SPECIAL_CHAR_CMD:
                ch = vt_esc_special_process(pvt, ch);
                if (ch != EOF){
                    pvt->is_esc = false;
                }
                break;
            case VT_SPECIAL_CHAR_G0:
            case VT_SPECIAL_CHAR_G1:
                pvt->is_esc = false;
                break;
            default:
                ch = vt_esc_char_process(pvt, ch);
                if (ch != EOF) pvt->is_esc = false;
        }
        return ch;
    }
    else if (ch == KEY_ESC){
        pvt->is_esc = true;
        ch = EOF;
    }

    return ch;
}





//***************************
// Transmit cursor visible or not
//
//
//***************************
char *vtResetDevice(vt100_t *pvt)
{
    snprintf(pvt->out_buffer, VT_ESC_ELEM_SIZE, "%s%c",VT_ESC_SEQ, VT_CMD_RESET);
    return pvt->out_buffer;
}



//***************************
// Send 'erase screen' sequence
//***************************
char *vtEraseScreen(vt100_t *pvt, uint8_t type)
{
    snprintf(pvt->out_buffer, VT_ESC_ELEM_SIZE, "%s%c%c",VT_ESC_SEQ, type, VT_CMD_ERASE_SCREEN);
    return pvt->out_buffer;
}


//***************************
// Send 'erase line' sequence
//***************************
char *vtEraseLine(vt100_t *pvt, uint8_t type)
{
    snprintf(pvt->out_buffer, VT_ESC_ELEM_SIZE, "%s%c%c",VT_ESC_SEQ, type, VT_CMD_ERASE_LINE);
    return pvt->out_buffer;
}


//***************************
// Send 'erase line' sequence
//***************************
char *vtEraseTab(vt100_t *pvt, uint8_t num_tab)
{
    snprintf(pvt->out_buffer, VT_ESC_ELEM_SIZE, "%s%d%c",VT_ESC_SEQ, num_tab, VT_CMD_ERASE_TAB);
    return pvt->out_buffer;
}


//***************************
// Send 'resize screen' sequence
//***************************
char *vtResizeScreen(vt100_t *pvt, uint16_t nrows, uint16_t ncols)
{
    if(snprintf(pvt->out_buffer, VT_ESC_ELEM_SIZE, "%s8;%d;%dt",VT_ESC_SEQ, nrows, ncols)) {
        pvt->nrows = nrows;
        pvt->ncols = ncols;
    }

    return pvt->out_buffer;
}


//***************************
// Set text colour
//***************************
char *vtSetColour(vt100_t *pvt, uint8_t fg_bg, uint8_t colour)
{
    snprintf(pvt->out_buffer, VT_ESC_ELEM_SIZE, "%s%c%c%c",VT_ESC_SEQ, fg_bg, colour, VT_CMD_ATTR);
    return pvt->out_buffer;
}


//***************************
// Set cursor position
//
// Top-left is (1,1)
//***************************
char *vtSetCursor(vt100_t *pvt, uint16_t row, uint16_t col)
{
    snprintf(pvt->out_buffer, VT_ESC_ELEM_SIZE, "%s%d;%d%c",VT_ESC_SEQ, row, col, VT_CMD_CURSOR);return pvt->out_buffer;
    return pvt->out_buffer;
}


//***************************
// Move cursor X to UP, DOWN, LEFT, RIGHT
//***************************
char *vtMoveCursor(vt100_t *pvt, uint8_t num, uint8_t cmd)
{
    snprintf(pvt->out_buffer, VT_ESC_ELEM_SIZE, "%s%d%c",VT_ESC_SEQ, num, cmd);
    return pvt->out_buffer;
}



//***************************
// Save cursor position
//***************************
char *vtSaveCursor(vt100_t *pvt)
{
    snprintf(pvt->out_buffer, VT_ESC_ELEM_SIZE, "%s%c",VT_ESC_SEQ, VT_CMD_CURSOR_SAVE);
    return pvt->out_buffer;
}


//***************************
// Restore cursor position
//***************************
char *vtRestoreCursor(vt100_t *pvt)
{
    snprintf(pvt->out_buffer, VT_ESC_ELEM_SIZE, "%s%c", VT_ESC_SEQ, VT_CMD_CURSOR_RESTORE);
    return pvt->out_buffer;
}


//***************************
// Invoke cursor position
//***************************
char *vtInvokeCursor(vt100_t *pvt)
{
    snprintf(pvt->out_buffer, VT_ESC_ELEM_SIZE, "%s6n",VT_ESC_SEQ);
    return pvt->out_buffer;
}


//***************************
// Region screen scrolling to some lines only
//***************************
char *vtSetScrollRegion(vt100_t *pvt, uint8_t start, uint8_t end)
{
    snprintf(pvt->out_buffer, VT_ESC_ELEM_SIZE, "%s%d;%d%c",VT_ESC_SEQ, start, end, VT_CMD_SCROLL);
    return pvt->out_buffer;
}



//***************************
// Set terminal mode and attribute
//***************************
char *vtChangeModeAttr(vt100_t *pvt, const char *mode, uint8_t cmd)
{
    snprintf(pvt->out_buffer, VT_ESC_ELEM_SIZE, "%s%s%c",VT_ESC_SEQ, mode, cmd);
    return pvt->out_buffer;
}





