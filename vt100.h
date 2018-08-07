/*****************************************************************//**
* @file
* @brief C File vt100.h
* @details      Source code for TODO
*
* @author Auban le Grelle
*
* @date 8 mai 2014 20:28:33
*
* <B>Contact:</B> a.legrelle@lgelectronicsystems.com
*
* @Copyright (c) 2012, Electronic Systems
*
* <B>Distribution:</B> This file is part of ccarm.
*
*    ccarm is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    ccarm is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General Public License
*    along with ccarm.  If not, see <http://www.gnu.org/licenses/>.
*
*
*********************************************************************/

#ifndef _VT100_H
#define _VT100_H

#include <stdbool.h>
#include <stdint.h>



#ifdef __cplusplus
extern "C" {
#endif


//Reference VT100 http://vt100.net/docs/vt220-rm/chapter4.html

#define VT_DEFAULT_NROWS        132     //!< Default Size number rows
#define VT_DEFAULT_NCOLS        80      //!< Default Size number columns

#define VT_ESC_ELEM_SIZE        16      //!< Buffer for escape sequence.



// Ascii Keyboard Key Extended
#define KB_F1               (0X100|59)
#define KB_F2               (0X100|60)
#define KB_F3               (0X100|61)
#define KB_F4               (0X100|62)
#define KB_F5               (0X100|63)
#define KB_F6               (0X100|64)
#define KB_F7               (0X100|65)
#define KB_F8               (0X100|66)
#define KB_F9               (0X100|67)
#define KB_F10              (0X100|68)
#define KB_F11              (0X100|133)
#define KB_F12              (0X100|134)

#define KB_SHIFT_F1         (0X100|84)
#define KB_SHIFT_F2         (0X100|85)
#define KB_SHIFT_F3         (0X100|86)
#define KB_SHIFT_F4         (0X100|87)
#define KB_SHIFT_F5         (0X100|88)
#define KB_SHIFT_F6         (0X100|89)
#define KB_SHIFT_F7         (0X100|90)
#define KB_SHIFT_F8         (0X100|91)
#define KB_SHIFT_F9         (0X100|92)
#define KB_SHIFT_F10        (0X100|93)
#define KB_SHIFT_F11        (0X100|135)
#define KB_SHIFT_F12        (0X100|136)

#define KB_CTRL_F1          (0X100|94)
#define KB_CTRL_F2          (0X100|95)
#define KB_CTRL_F3          (0X100|96)
#define KB_CTRL_F4          (0X100|97)
#define KB_CTRL_F5          (0X100|98)
#define KB_CTRL_F6          (0X100|99)
#define KB_CTRL_F7          (0X100|100)
#define KB_CTRL_F8          (0X100|101)
#define KB_CTRL_F9          (0X100|102)
#define KB_CTRL_F10         (0X100|103)
#define KB_CTRL_F11         (0X100|137)
#define KB_CTRL_F12         (0X100|138)

#define KB_ALT_F1           (0X100|104)
#define KB_ALT_F2           (0X100|105)
#define KB_ALT_F3           (0X100|106)
#define KB_ALT_F4           (0X100|107)
#define KB_ALT_F5           (0X100|108)
#define KB_ALT_F6           (0X100|109)
#define KB_ALT_F7           (0X100|110)
#define KB_ALT_F8           (0X100|111)
#define KB_ALT_F9           (0X100|112)
#define KB_ALT_F10          (0X100|113)
#define KB_ALT_F11          (0X100|139)
#define KB_ALT_F12          (0X100|140)

#define KB_UP               (0X100|72)
#define KB_LEFT             (0X100|75)
#define KB_RIGHT            (0X100|77)
#define KB_DOWN             (0X100|80)
#define KB_HOME             (0X100|71)
#define KB_END              (0X100|79)
#define KB_PGUP             (0X100|73)
#define KB_PGDN             (0X100|81)
#define KB_INSERT           (0X100|82)
#define KB_DELETE           (0X100|83)

#define KB_CTRL_UP          (0X100|132)
#define KB_CTRL_LEFT        (0X100|115)
#define KB_CTRL_RIGHT       (0X100|116)
#define KB_CTRL_DOWN        (0X100|145)
#define KB_CTRL_HOME        (0X100|119)
#define KB_CTRL_END         (0X100|117)
#define KB_CTRL_PGUP        (0X100|132)
#define KB_CTRL_PGDN        (0X100|118)
#define KB_CTRL_INSERT      (0X100|146)
#define KB_CTRL_DELETE      (0X100|147)

#define KB_ALT_UP           (0X100|152)
#define KB_ALT_LEFT         (0X100|155)
#define KB_ALT_RIGHT        (0X100|157)
#define KB_ALT_DOWN         (0X100|154)
#define KB_ALT_HOME         (0X100|151)
#define KB_ALT_END          (0X100|159)
#define KB_ALT_PGUP         (0X100|153)
#define KB_ALT_PGDN         (0X100|161)
#define KB_ALT_INSERT       (0X100|162)
#define KB_ALT_DELETE       (0X100|163)



/*** Ascii Key codes ***/
#define KEY_NUL      0   //!< ^@ Null character
#define KEY_SOH      1   //!< ^A Start of heading, = console interrupt
#define KEY_STX      2   //!< ^B Start of text, maintenance mode on HP console
#define KEY_ETX      3   //!< ^C End of text
#define KEY_EOT      4   //!< ^D End of transmission, not the same as ETB
#define KEY_ENQ      5   //!< ^E Inquiry, goes with ACK; old HP flow control
#define KEY_ACK      6   //!< ^F Acknowledge, clears ENQ logon hand
#define KEY_BEL      7   //!< ^G Bell, rings the bell...
#define KEY_BS       8   //!< ^H Backspace, works on HP terminals/computers
#define KEY_HT       9   //!< ^I Horizontal tab, move to next tab stop
#define KEY_LF       10  //!< ^J Line Feed
#define KEY_VT       11  //!< ^K Vertical tab
#define KEY_FF       12  //!< ^L Form Feed, page eject
#define KEY_CR       13  //!< ^M Carriage Return
#define KEY_SO       14  //!< ^N Shift Out, alternate character set
#define KEY_SI       15  //!< ^O Shift In, resume defaultn character set
#define KEY_DLE      16  //!< ^P Data link escape
#define KEY_DC1      17  //!< ^Q XON, with XOFF to pause listings; "okay to send".
#define KEY_DC2      18  //!< ^R Device control 2, block-mode flow control
#define KEY_DC3      19  //!< ^S XOFF, with XON is TERM=18 flow control
#define KEY_DC4      20  //!< ^T Device control 4
#define KEY_NAK      21  //!< ^U Negative acknowledge
#define KEY_SYN      22  //!< ^V Synchronous idle
#define KEY_ETB      23  //!< ^W End transmission block, not the same as EOT
#define KEY_CAN      24  //!< ^X Cancel line, MPE echoes !!!
#define KEY_EM       25  //!< ^Y End of medium, Control-Y interrupt
#define KEY_SUB      26  //!< ^Z Substitute
#define KEY_ESC      27  //!< ^[ Escape, next character is not echoed
#define KEY_FS       28  //!< ^\ File separator
#define KEY_GS       29  //!< ^] Group separator
#define KEY_RS       30  //!< ^^ Record separator, block-mode terminator
#define KEY_US       31  //!< ^_ Unit separator
#define KEY_DEL      127 //!< Delete (not a real control character...)



/*** Terminal Mode Command ***/
#define VT_CMD_MODE_SET         'h'
#define VT_CMD_MODE_RESET       'l'

/*** Text Color Command ***/
#define VT_CMD_COL_FOREGROUND   '3'     //!< Set attr color foreground
#define VT_CMD_COL_BACKGROUND   '4'     //!< Set attr color background



/*** Terminal Mode and Attribute ***/
#define VT_MODE_KAM            "2"      //!< Lock/Unlock Keyboard
#define VT_MODE_IRM            "4"      //!< Insert/Replace for add char
#define VT_MODE_SRM            "12"     //!< Turn Local Echo ON/OFF
#define VT_MODE_LNM            "20"     //!< Line feed mode set = CRLF, reset = CR
#define VT_MODE_TCE            "25"     //!< Cursor Visible/Invisible
#define VT_MODE_CKM            "?1"     //!< Cursor Key APP CONTROL/ ANSI CONTROL
#define VT_MODE_AVM            "?2"     //!< ANSI/VT52 Mode only set
#define VT_MODE_COLM           "?3"     //!< Number column per line 132/80 column
#define VT_MODE_SCLM           "?4"     //!< Scroll mode 6lines/sec or direct jump.
#define VT_MODE_SCNM           "?5"     //!< Selects reverse video or Selects normal screen
#define VT_MODE_OM             "?6"     //!< Origin mode scroll in region ON/OFF
#define VT_MODE_AWM            "?7"     //!< Auto Wrap  ON/OFF
#define VT_MODE_ARM            "?8"     //!< Auto Wrap  ON/OFF
#define VT_MODE_PFF            "?18"    //!< Select termination char (FF) ON/OFF
#define VT_MODE_PEX            "?19"    //!< Selects full screen or scroll region to print

#define VT_MODE_ATTR_NONE       '0'     //!< Reset all attributes
#define VT_MODE_ATTR_BOLD       '1'     //!< Set "bright" attribute
#define VT_MODE_ATTR_DIM        '2'     //!< Set "dim" attribute
#define VT_MODE_ATTR_UNDERLINE  '4'     //!< Set "underscore" (underlined text) attribute
#define VT_MODE_ATTR_BLINK      '5'     //!< Set "blink" attribute
#define VT_MODE_ATTR_REVERSED   '7'     //!< Set "reverse" attribute
#define VT_MODE_ATTR_CONCEALED  '8'     //!< Set "hidden" attribute
#define VT_MODE_ATTR_DNI        "22"    //!< Display normal intensity.
#define VT_MODE_ATTR_DNU        "24"    //!< Display not underlined.
#define VT_MODE_ATTR_DNB        "25"    //!< Display not blinking.
#define VT_MODE_ATTR_DPI        "27"    //!< Display positive image.




/*** Move Cursor ***/
#define VT_MOVE_CUR_UP          'A'     //!< Move Cursor UP
#define VT_MOVE_CUR_DOWN        'B'     //!< Move Cursor DOWN
#define VT_MOVE_CUR_RIGHT       'C'     //!< Move Cursor RIGHT
#define VT_MOVE_CUR_LEFT        'D'     //!< Move Cursor LEFT
#define VT_MOVE_CUR_NL          'E'     //!< Move Cursor Next Line
#define VT_MODE_CUR_PL          'F'     //!< Move Cursor Previous Line
#define VT_MOVE_CUR_H           'G'     //!< Horizontal Absolute Position
#define VT_MOVE_CUR_V           'd'     //!< Vertical Absolute Position

#define VT_ERASE_CHAR           'X'     //!< Erase character
#define VT_HORIZ_TAB            'I'     //!< Horizontal Tab
#define VT_BACK_TAB             'Z'     //!< Backward Tab
#define VT_INSERT_LINE          'L'     //!< Insert Line
#define VT_DEL_LINE             'M'     //!< Delete Line
#define VT_INSERT_CHAR          '@'     //!< Insert Character
#define VT_DELETE_CHAR          'P'     //!< Delete Character




/*** Erasing text ***/
#define VT_ERASE_LINE_END       '0'     //!< Erases from cursor to end line.
#define VT_ERASE_LINE_START     '1'     //!< Erases from cursor to the start line.
#define VT_ERASE_LINE_ALL       '2'     //!< Erases the entire current line.

#define VT_ERASE_SCREEN_DOWN    '0'     //!< Erases the screen from line to down.
#define VT_ERASE_SCREEN_UP      '1'     //!< Erases the screen from line to top.
#define VT_ERASE_SCREEN_ALL     '2'     //!< Erases the screen set cursor home.




/*** Text Color ***/
#define VT_COL_BLACK            '0'     //!< Set color #0 - black
#define VT_COL_RED              '1'     //!< Set color #1 - red
#define VT_COL_GREEN            '2'     //!< Set color #2 - green
#define VT_COL_YELLOW           '3'     //!< Set color #3 - yellow
#define VT_COL_BLUE             '4'     //!< Set color #4 - blue
#define VT_COL_MAGENTA          '5'     //!< Set color #5 - magenta
#define VT_COL_CYAN             '6'     //!< Set color #6 - cyan
#define VT_COL_WHITE            '7'     //!< Set color #7 - white
#define VT_COL_DEFAULT          '9'     //!< Set default color




// vt struct
struct vt100{
   uint16_t             nrows;                      //!< Number of Rows
   uint16_t             ncols;                      //!< Number of Columns
   uint16_t             row_pos;
   uint16_t             col_pos;
   char                 out_buffer[VT_ESC_ELEM_SIZE];
   bool               	is_esc;                     //!< Is in escape sequence
   bool               	esc_read_num;
   int32_t              esc_cur_num;
   int32_t              esc_char;
   uint8_t              esc_elems;
   uint8_t              esc_elem[VT_ESC_ELEM_SIZE]; //!< Escape Sequence buffer
};
typedef struct vt100 vt100_t;









int32_t vtProcessChar(vt100_t *pvt, int32_t ch);
char * vtResetDevice(vt100_t *pvt);
char * vtSetCursorVisible(vt100_t *pvt, bool state);

char *vtEraseScreen(vt100_t *pvt, uint8_t type);
char *vtEraseTab(vt100_t *pvt, uint8_t num_tab);
char *vtEraseLine(vt100_t *pvt, uint8_t type);

char *vtResizeScreen(vt100_t *pvt, uint16_t nrows, uint16_t ncols);
char *vtSetColour(vt100_t *pvt, uint8_t fg_bg, uint8_t colour);

char *vtSetCursor(vt100_t *pvt, uint16_t row, uint16_t col);
char *vtMoveCursor(vt100_t *pvt, uint8_t num, uint8_t cmd);
char *vtSaveCursor(vt100_t *pvt);
char *vtRestoreCursor(vt100_t *pvt);
char *vtInvokeCursor(vt100_t *pvt);

char *vtSetScrollRegion(vt100_t *pvt, uint8_t start, uint8_t end);
char *vtChangeModeAttr(vt100_t *pvt, const char *mode, uint8_t cmd);



#ifdef __cplusplus
}
#endif


#endif /* _VT100_H */
