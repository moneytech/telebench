/*==============================================================================
 *$RCSfile: thfli.h,v $
 *
 *   DESC : Test Harness routines for benchmarks
 *
 * AUTHOR : ECL, LLC
 *
 *  EEMBC : Technical Advisory Group (TechTAG)
 *
 *    CVS : $Revision: 1.6 $
 *          $Date: 2002/08/06 21:57:16 $
 *          $Author: rick $
 *          $Source: d:/cvs/eembc2/techtag/th/src/thfli.h,v $
 *          
 * NOTE   :
 *
 *------------------------------------------------------------------------------
 *
 * HISTORY :
 *
 * $Log: thfli.h,v $
 * Revision 1.6  2002/08/06 21:57:16  rick
 * Add NI CRC to TH Regular
 *
 * Revision 1.5  2002/07/19 19:29:03  rick
 * Replace eembc_id string with Benchmark ID
 *
 * Revision 1.4  2002/04/23 18:33:57  rick
 * Match eembc_dt with Lite, remove stdinc.h
 *
 * Revision 1.3  2002/03/22 18:25:25  rick
 * Add uuencode to Networking th regular
 *
 * Revision 1.2  2002/03/21 22:31:10  rick
 * Network Harness, and empty benchmark fixes
 *
 *
 *------------------------------------------------------------------------------
 * Copyright (c) 1998-2002 by the EDN Embedded Microprocessor 
 * Benchmark Consortium (EEMBC), Inc. 
 * 
 * All Rights Reserved. This is licensed program product and 
 * is owned by EEMBC. The Licensee understands and agrees that the 
 * Benchmarks licensed by EEMBC hereunder (including methods or concepts 
 * utilized therein) contain certain information that is confidential 
 * and proprietary which the Licensee expressly agrees to retain in the 
 * strictest confidence and to use only in conjunction with the Benchmarks 
 * pursuant to the terms of this Agreement. The Licensee further agrees 
 * to keep the source code and all related documentation confidential and 
 * not to disclose such source code and/or related documentation to any 
 * third party. The Licensee and any READER of this code is subject to 
 * either the EEMBC Member License Agreement and/or the EEMBC Licensee 
 * Agreement. 
 * TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, EEMBC DISCLAIMS ALL 
 * WARRANTIES, EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, 
 * IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR 
 * PURPOSE, WITH REGARD TO THE BENCHMARKS AND THE ACCOMPANYING 
 * DOCUMENTATION. LICENSEE ACKNOWLEDGES AND AGREES THAT THERE ARE NO 
 * WARRANTIES, GUARANTIES, CONDITIONS, COVENANTS, OR REPRESENTATIONS BY 
 * EEMBC AS TO MARKETABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR OTHER 
 * ATTRIBUTES, WHETHER EXPRESS OR IMPLIED (IN LAW OR IN FACT), ORAL OR 
 * WRITTEN. 
 * 
 * Licensee hereby agrees by accessing this source code that all benchmark 
 * scores related to this code must be certified by ECL prior to publication 
 * in any media, form, distribution, or other means of conveyance of 
 * information subject to the terms of the EEMBC Member License Agreement 
 * and/or EEMBC Licensee Agreement. 
 * 
 * Other Copyright Notice (if any): 
 * 
 * For conditions of distribution and use, see the accompanying README file.
 * ===========================================================================*/

#ifndef   THFLI_H       /* File Sentinal */
#define   THFLI_H

#include "thlib.h"
#include <stdarg.h>       /* Needed for printf support */

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
 *            **** Test Harness Functional Layer Interface ****
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Function Types
*/

/* The 'thft_'  stands for 'test harness function type' */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * Console I/O Functions
 *
 * These functions provide I/O to a logical console.  Presumeably, this is
 * a console 'window' or 'session' on the host system
*/

typedef int (*thft_printf) ( const char *fmt, va_list args );
typedef int (*thft_sprintf) ( char *str, const char *fmt, va_list args );
typedef int (*thft_sends) ( const char *str );
typedef int (*thft_putchar) ( char c );
typedef int (*thft_write_con) ( const char *buf, size_t buf_size );
typedef size_t( *thft_read_con ) ( char *buf, size_t buf_size );
typedef size_t (*thft_con_chars_avail) ( void );

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * Target Timer Support
 *
 * These funtions provide access to the target's timer (if it is available)
*/

typedef size_t (*thft_ticks_per_sec) ( void );
typedef size_t (*thft_tick_granularity) ( void );

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * Target memory allocation support
 *
 * These functions provide an interace into the test harness' memory
 * allocation routines
*/

typedef void * ( *thft_malloc ) ( size_t size, const char *file, int line );
typedef void (*thft_free) ( void *block, const char *file, int line );
typedef void (*thft_heap_reset) ( void );

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * Execution Control
*/

typedef void (*thft_signal_start) ( void );
typedef size_t (*thft_signal_finished) ( void );

typedef void (*thft_exit) ( int exit_code, const char *fmt, va_list args );

typedef int (*thft_report_results) ( const THTestResults * results, e_u16 Expected_CRC );

typedef int (*thft_harness_poll) ( void );

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * File Handling
*/

typedef FileDef * ( *thft_get_file_def ) ( const char *fn );
typedef FileDef * ( *thft_get_file_num ) ( int n );

typedef int (*thft_send_buf_as_file)
                     ( const char* buf, BlockSize length, const char* fn );

/*------------------------------------------------------------------------------
 * Structures and Typedefs

 * == Test Harness Defintion. ==
 *
 * A const pointer to this sturcture is passed to the test component's
 * entry point.  The TC uses the information in this structure to
 *
 *     - determine the identity and other info regarding the tareget system
 *     - determine the identity and version of the test harness
 *     - determine the capeabilities of the target system
 *     - determine the capeabilities of the test harness
 *     - obtain pointers to the test harness entry points
*/

/* THDef.revsion == 4  { this is revision 2 of this structure } */

#define THDEF_REVISION (4)

typedef struct THDef

   {
   /* Values passed from the Test Harness to the Test Library
   */
   char eembc_bm_id[ 16 ];/* id flag */
   char desc[ 64 ];       /* benchmark description */
   char member[ 16 ];     /* the member id */
   char processor[ 16 ];  /* the processor id */
   char platform[ 16 ];   /* the platform id */

   Word              revision;        /* The revision of this structure. */

   version_number    th_vnum;         /* the test harness version number */
   version_number    target_vnum;     /* the target hardware version number */

   void *target_info;                 /* target specific information */

   int               target_timer_avail;
   int               target_timer_is_intrusive;

   /* Test Harness Interface pointers  ( thips... :)
    *    These functions are called by the Test Library and are implimented
    *    by the test harness itself.
   */
   thft_printf                 thip_printf;
   thft_sprintf                thip_sprintf;
   thft_sends                  thip_sends;
   thft_putchar                thip_putchar;
   thft_write_con              thip_write_con;
   thft_read_con               thip_read_con;
   thft_con_chars_avail        thip_con_chars_avail;

   thft_ticks_per_sec          thip_ticks_per_sec;
   thft_tick_granularity       thip_tick_granularity;

   thft_malloc                 thip_malloc;
   thft_free                   thip_free;
   thft_heap_reset             thip_heap_reset;

   thft_signal_start           thip_signal_start;
   thft_signal_finished        thip_signal_finished;
   thft_exit                   thip_exit;
   thft_report_results         thip_report_results;
   thft_harness_poll           thip_harness_poll;

   thft_get_file_def           thip_get_file_def;
   thft_get_file_num           thip_get_file_num;

   thft_send_buf_as_file       thip_send_buf_as_file;
   }
THDef;

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
   }
#endif

#endif                /* File Sentinal */

