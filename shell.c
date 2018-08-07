/***************************************************************************//**
* @file
* @brief C File shell.c
* @details Source code for TODOdescription
*
* @author Auban le Grelle
*
* @date 7 mai 2014 10:23:13
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
*    along with Embedd
*    edLib.  If not, see <http://www.gnu.org/licenses/>.
*
*******************************************************************************/

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "shell.h"





//Declare Prototype
static inline void shell_print_prompt(shellObject_t *pshell);
static void shell_insert_char(shellObject_t *pshell, char ch);
static void shell_remove_char(shellObject_t *pshell);
static void shell_move_cursor_right(shellObject_t *pshell);
static void shell_move_cursor_left(shellObject_t *pshell);

static void shell_handle_history(shellObject_t *pshell);
static void shell_push_history(shellObject_t *pshell);

static size_t shell_read(shellObject_t *pshell);








//Private Function
//*****************************************************************************
static inline void shell_print_prompt(shellObject_t *pshell)
{
    pshell->line_cur = 0;
    pshell->line_pos = 0;
    pshell->line[0] = 0;
    shellPrintf(pshell, pshell->prompt);
}


//*****************************************************************************
// insert len char of text at cursor position
static void shell_insert_char(shellObject_t *pshell, char ch)
{
    int i;

    /* it's a large line, discard it */
    if (pshell->line_pos < sizeof(pshell->line) - 1){
        /* normal character */
        if (pshell->line_cur < pshell->line_pos)
        {
            memmove(&pshell->line[pshell->line_cur + 1],
                    &pshell->line[pshell->line_cur],
                    pshell->line_pos - pshell->line_cur);

            pshell->line[pshell->line_cur] = ch;
            pshell->line[pshell->line_pos + 1] = 0;

            if (pshell->echo){
                shellPrintf(pshell, &pshell->line[pshell->line_cur]);

                /* move the cursor to new position */
                for (i = pshell->line_cur; i < pshell->line_pos; i++) {
                    shellPutc(KEY_BS, pshell);
                }
            }
        }
        else
        {
            pshell->line[pshell->line_pos] = ch;
            if (pshell->echo) shellPutc(ch, pshell);
            pshell->line[pshell->line_pos + 1] = 0;
        }

        pshell->line_pos++;
        pshell->line_cur++;
    }
}


//*****************************************************************************
// insert len char of text at cursor position
static void shell_remove_char(shellObject_t *pshell)
{
    int i;

    if(pshell->line_cur > 0) {
        pshell->line_cur--;
        pshell->line_pos--;

        if (pshell->line_pos > pshell->line_cur)
        {
            memmove(&pshell->line[pshell->line_cur],
                    &pshell->line[pshell->line_cur + 1],
                    pshell->line_pos - pshell->line_cur);

            pshell->line[pshell->line_pos] = 0;

            if (pshell->echo) {
                shellPutc(KEY_BS, pshell);
                shellPrintf(pshell, &pshell->line[pshell->line_cur]);
                shellPutc(' ', pshell);
                shellPrintf(pshell , vtMoveCursor(pshell->vt, 1, VT_DELETE_CHAR));

                /* move the cursor to the origin position */
                for (i = pshell->line_cur; i <= pshell->line_pos; i++) {
                    shellPutc(KEY_BS, pshell);
                }

            }
        }
        else
        {
            if (pshell->echo) {
                shellPutc(KEY_BS, pshell);
                shellPrintf(pshell , vtMoveCursor(pshell->vt, 1, VT_DELETE_CHAR));
            }
            pshell->line[pshell->line_pos] = 0;
        }
    }
}


//*****************************************************************************
// insert len char of text at cursor position
static void shell_move_cursor_right(shellObject_t *pshell)
{
    if(pshell->line_cur < pshell->line_pos){
        pshell->line_cur++;

        if (pshell->echo) {
            //Move cursor down to start of line
            if(!((pshell->line_cur + strlen(pshell->prompt)) % pshell->vt->ncols)) {
                shellPrintf(pshell ,  vtMoveCursor(pshell->vt, 1, VT_MOVE_CUR_DOWN));
                shellPrintf(pshell , vtMoveCursor(pshell->vt, 1, VT_MOVE_CUR_H));
            }
            else shellPrintf(pshell , vtMoveCursor(pshell->vt, 1, VT_MOVE_CUR_RIGHT));
        }
    }
}


//*****************************************************************************
// insert len char of text at cursor position
static void shell_move_cursor_left(shellObject_t *pshell)
{
    if(pshell->line_cur > 0){
        if (pshell->echo) {
            //Move cursor up to end of line
            if(!((pshell->line_cur + strlen(pshell->prompt)) % pshell->vt->ncols)) {
                shellPrintf(pshell , vtMoveCursor(pshell->vt, 1, VT_MOVE_CUR_UP));
                shellPrintf(pshell , vtMoveCursor(pshell->vt, pshell->vt->ncols, VT_MOVE_CUR_H));
            }
            else shellPrintf(pshell , vtMoveCursor(pshell->vt, 1, VT_MOVE_CUR_LEFT));
        }

        pshell->line_cur--;
    }
}



static void shell_handle_history(shellObject_t *pshell)
{
   shellPrintf(pshell , vtEraseLine(pshell->vt, VT_ERASE_LINE_ALL));
   shellPrintf(pshell , "\r");
   shell_print_prompt(pshell);

   /* copy the history command */
   memcpy(pshell->line, &pshell->history[pshell->history_current][0],
          SHELL_HISTORY_CMD_SIZE);

   pshell->line_cur = pshell->line_pos = strlen(pshell->line);

   shellPrintf(pshell, pshell->line);
}

static void shell_push_history(shellObject_t *pshell)
{
    int index;

    if (pshell->line_pos != 0)
    {
        /* push history */
        if (pshell->history_count >= SHELL_HISTORY_LINES)
        {
            /* move history */
            for (index = 0; index < SHELL_HISTORY_LINES - 1; index ++)
            {
                memcpy(&pshell->history[index][0],
                    &pshell->history[index + 1][0], SHELL_HISTORY_CMD_SIZE);
            }
            memset(&pshell->history[index][0], 0, SHELL_HISTORY_CMD_SIZE);
            memcpy(&pshell->history[index][0], pshell->line, SHELL_HISTORY_CMD_SIZE - 1);

            /* it's the maximum history */
            pshell->history_count = SHELL_HISTORY_LINES;
        }
        else
        {
            memset(&pshell->history[pshell->history_count][0], 0, SHELL_HISTORY_CMD_SIZE);
            memcpy(&pshell->history[pshell->history_count][0], pshell->line, SHELL_HISTORY_CMD_SIZE - 1);

            /* increase count and set current history position */
            pshell->history_count++;
        }
    }

    pshell->history_current = pshell->history_count;
}




/* Return number character line*/
static size_t shell_read(shellObject_t *pshell)
{
    uint8_t tabnumchar;
    int32_t ch = 0;

    while (ch != EOF) {
        ch = vtProcessChar(pshell->vt, shellGetc(pshell));

        if (ch != EOF) {
            if (!isprint(ch)){
                switch(ch) {
                    case KB_UP:
                        /* prev history */

                        if (pshell->history_current > 0)
                            pshell->history_current --;
                        else
                        {
                            pshell->history_current = 0;
                            break;
                        }

                        shell_handle_history(pshell);

                        break;
                    case KB_DOWN:
                        /* next history */
                        if (pshell->history_current < pshell->history_count - 1) {
                            pshell->history_current++;
                        }
                        else {
                            /* set to the end of history */
                            if (pshell->history_count != 0) {
                                pshell->history_current = pshell->history_count - 1;
                            }
                            else {
                                break;
                            }
                        }

                        shell_handle_history(pshell);

                        break;
                    case KB_LEFT:
                        shell_move_cursor_left(pshell);
                        break;
                    case KB_RIGHT:
                        shell_move_cursor_right(pshell);
                        break;
                    case KEY_LF:
                        shellPutc(ch, pshell);

                        shell_push_history(pshell);

                        return strlen(pshell->line);
                    case KEY_FF:
                        shellPrintf(pshell , vtSetCursor(pshell->vt, 1, 1));
                        shellPrintf(pshell , vtEraseScreen(pshell->vt, VT_ERASE_SCREEN_ALL));
                        return 0;
                    case KEY_CR:
                        shellPutc(ch, pshell);
                        break;
                    case KEY_DEL:
                    case KEY_BS:
                        shell_remove_char(pshell);
                        break;
                    case KEY_HT:
                        if (pshell->echo) {
                            tabnumchar = pshell->vt->col_pos % SHELL_NUM_TAB;
                            if(!tabnumchar) tabnumchar = SHELL_NUM_TAB;
                            shellPrintf(pshell , vtMoveCursor(pshell->vt, tabnumchar, VT_MOVE_CUR_RIGHT));
                        }
                        break;
                    case KEY_VT:
                        if (pshell->echo) {
                            shellPrintf(pshell , vtMoveCursor(pshell->vt, 1, VT_MOVE_CUR_DOWN));
                        }
                        break;
                }
            }
            else {
                shell_insert_char(pshell, ch);
            }
        }
    }

    return -1;
}















/******************************************************************************/
//Public Function
shellObject_t *shellOpen(FILE *out, FILE *in, const char *prompt, shell_ops_t *ops)
{
    shellObject_t  *pshell;
    vt100_t *pvt100;

    //Create Instance of Shell Object
    pshell = (shellObject_t *) malloc(sizeof(shellObject_t));
    if (pshell == NULL) return NULL;

    pvt100 = (vt100_t *) malloc(sizeof(vt100_t));
    if (pvt100 == NULL) return NULL;


    //Set to 0 memory shell
    memset(pshell, 0, sizeof(shellObject_t));



    //Set prompt name
    pshell->prompt = prompt;

    //Set Default Echo
    pshell->echo = SHELL_DEFAULT_ECHO;

    pshell->in = in;
    pshell->out = out;
    pshell->vt = pvt100;

    pshell->ops = ops;

    return pshell;
}


s_err_t shellInit(shellObject_t *pshell, bool echo)
{
    pshell->echo = echo;

    shellPrintf(pshell , vtResizeScreen(pshell->vt,
                SHELL_DEFAULT_NROWS, SHELL_DEFAULT_NCOLS));    //Resize Screen
    shellPrintf(pshell , vtChangeModeAttr(pshell->vt,
                VT_MODE_SRM, VT_CMD_MODE_SET));                //Local Echo OFF
    shellPrintf(pshell , vtChangeModeAttr(pshell->vt ,
                VT_MODE_LNM, VT_CMD_MODE_SET));               //Line Feed CRLF

    if (pshell->ops != NULL) {
        pshell->ops->start_shell();
    }

    shell_print_prompt(pshell);

    shellPrintf(pshell , vtInvokeCursor(pshell->vt));


    return SYS_EOK;
}


s_err_t shellClose(shellObject_t *pshell)
{
    shellPrintf(pshell , vtEraseScreen(pshell->vt, VT_ERASE_SCREEN_ALL));

    free(pshell->vt);
    free(pshell);

    return SYS_EOK;
}


void shellPrintf(shellObject_t *pshell, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    vfprintf(pshell->out, fmt, args);

    va_end(args);
}



inline int32_t shellGetc(shellObject_t *pshell)
{
    int32_t ch;

    ch = fgetc(pshell->in);
    clearerr(pshell->in);

    if (ch == EOF){
        return EOF;
    }

    return ch;
}


inline int32_t shellPutc(int32_t ch, shellObject_t *pshell)
{
    return fputc(ch, pshell->out);
}




char *shellEngine(shellObject_t *pshell)
{
    size_t nchar;

    switch(pshell->state){
        case SHELL_STATE_START:
            pshell->state++;
            break;
        case SHELL_STATE_LOGIN:
            pshell->state++;
            break;
        case SHELL_STATE_READY:
            nchar = shell_read(pshell);
            if(nchar != -1) {
                pshell->state++;
                return pshell->line;
            }
            break;
        case SHELL_STATE_RX_CMD:
            shell_print_prompt(pshell);
            pshell->state = SHELL_STATE_READY;
            break;
        default:
            pshell->state = 0;
    }

    return NULL;
}




