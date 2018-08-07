/*****************************************************************//**
* @file
* @brief H File shell.h
* @details This file is the header of the shell terminal
*
* @author Auban le Grelle
*
* @date 10 mar. 2014 22:28:17
*
* <B>Contact:</B> a.legrelle@lgelectronicsystems.com
*
* @copyright (c) 2012, Electronic Systems
*
* <B>Distribution:</B> This file is part of dvbox.
*
*    dvbox is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    dvbox is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General Public License
*    along with dvbox.  If not, see <http://www.gnu.org/licenses/>.
*
*
*******************************************************************************/

#ifndef _SHELL_H
#define _SHELL_H


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "vt100.h"


#ifdef __cplusplus
extern "C" {
#endif

/* Error code definitions */
#ifndef SYS_EOK
#define SYS_EOK                          0      //!< There is no error
#endif
#ifndef SYS_ERROR
#define SYS_ERROR                        1      //!< A generic error happens
#endif
#ifndef SYS_ETIMEOUT
#define SYS_ETIMEOUT                     2      //!< Timed out
#endif
#ifndef SYS_EFULL
#define SYS_EFULL                        3      //!< The resource is full
#endif
#ifndef SYS_EEMPTY
#define SYS_EEMPTY                       4      //!< The resource is empty
#endif
#ifndef SYS_ENOMEM
#define SYS_ENOMEM                       5      //!< No memory
#endif
#ifndef SYS_ENOSYS
#define SYS_ENOSYS                       6      //!< No system
#endif
#ifndef SYS_EBUSY
#define SYS_EBUSY                        7      //!< Busy
#endif
#ifndef SYS_EIO
#define SYS_EIO                          8      //!< IO error
#endif
#ifndef SYS_EINT
#define SYS_EINT                         9      //!< Interrupted system
#endif


#define SHELL_DEFAULT_NROWS              50
#define SHELL_DEFAULT_NCOLS              80




#define SHELL_DEFAULT_ECHO               true


#define SHELL_NUM_TAB                    4
#define SHELL_BUFFER_LINE_LEN            (100)     //!< Command line length + null terminator



#ifndef SHELL_HISTORY_LINES
#define SHELL_HISTORY_LINES             10
#endif

#ifndef SHELL_HISTORY_CMD_SIZE
#define SHELL_HISTORY_CMD_SIZE          32
#endif


#define SHELL_STATE_START               0
#define SHELL_STATE_LOGIN               1
#define SHELL_STATE_READY               2
#define SHELL_STATE_RX_CMD              3



typedef uint8_t s_err_t;       				/**< Type for error number */

/**
 * Shell operations Structure
 */
struct shell_ops
{
    /* start shell callback */
    void    (*start_shell)(void);
};
typedef struct shell_ops shell_ops_t;


/**
 * Shell Object Structure
 */
struct shellObject{
    char                line[SHELL_BUFFER_LINE_LEN];   //!< Buffer Line
    uint16_t            line_pos;
    uint16_t            line_cur;
    const char          *prompt;
    bool                echo;
    uint8_t             state;

    uint16_t            history_current;
    uint16_t            history_count;
    char                history[SHELL_HISTORY_LINES][SHELL_HISTORY_CMD_SIZE];

    FILE                *in;
    FILE                *out;
    vt100_t             *vt;
    shell_ops_t         *ops;
};
typedef struct shellObject shellObject_t;




shellObject_t *shellOpen(FILE *out, FILE *in, const char *prompt, shell_ops_t *ops);
s_err_t shellInit(shellObject_t *pshell, bool echo);
s_err_t shellClose(shellObject_t *pshell);
void shellPrintf(shellObject_t *pshell, const char *fmt, ...);
int32_t shellGetc(shellObject_t *pshell);
int32_t shellPutc(int32_t ch, shellObject_t *pshell);
char *shellEngine(shellObject_t *pshell);





#ifdef __cplusplus
}
#endif

#endif /* _CONSOLE_H */
