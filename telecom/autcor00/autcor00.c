/*==============================================================================
 *$RCSfile: autcor00.c,v $
 *
 *   DESC : Fixed Point AutoCorrelation
 *
 * AUTHOR : 
 *
 *  EEMBC : Telecom Subcommittee 
 *
 *    CVS : $Revision: 1.4 $
 *          $Date: 2002/04/01 20:51:22 $
 *          $Author: administrator $
 *          $Source: d:/cvs/eembc2/telecom/autcor00/autcor00.c,v $
 *          
 * NOTE   :
 *
 *------------------------------------------------------------------------------
 *
 * HISTORY :
 *
 * $Log: autcor00.c,v $
 * Revision 1.4  2002/04/01 20:51:22  administrator
 * Apply Standard Comment Block
 *
 * Revision 1.3  2002/03/11 22:11:50  rick
 * ITERATIONS, CRC_CHECK, NON_INTRUSIVE TCDef Usage
 *
 * Revision 1.2  2002/02/18 23:51:49  rick
 * Completed Telocom TH_Lite conversion
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

/*******************************************************************************
    Includes                                                                    
*******************************************************************************/
#include "algo.h"

/*******************************************************************************
    Functions                                                                   
*******************************************************************************/
/*------------------------------------------------------------------------------
 * FUNC    : fxpAutoCorrelation
 *
 * DESC    : 
 *
 * Compute the autocorrelation of the InputData (size = DataSize),
 * store the results in AutoCorrData (size = NumberOfLags).
 * For this application NumberOfLags is small (<64) so a direct
 * sum-of-products implementation is used to compute the output.
 * Partial products are scaled by Scale bits.
 *
 *         
 * RETURNS : 
 *      true/false
 * ---------------------------------------------------------------------------*/

void
fxpAutoCorrelation (
    e_s16   *InputData,     /* input data */
    e_s16   *AutoCorrData,  /* output data */
    e_s16   DataSize,       /* size of input data */
    e_s16   NumberOfLags,   /* size of output data */
    e_s16   Scale           /* partial product scale (bits) */
)
{
    n_int   i;
    n_int   lag;
    n_int   LastIndex;
    e_s32    Accumulator;

    /* Compute AutoCorrelation */
    for (lag = 0; lag < NumberOfLags; lag++) {
        Accumulator = 0;
        LastIndex = DataSize - lag;
        for (i = 0; i < LastIndex; i++) {
            Accumulator += ((e_s32) InputData[i] * (e_s32) InputData[i+lag]) >> Scale;
        }

        /* Extract MSW of 1.31 fixed point accumulator */
        AutoCorrData[lag] = (e_s16) (Accumulator >> 16) ;
    }
}

