/**
 *
 * Copyright (C) EEMBC(R) All Rights Reserved
 *
 * This software is licensed with an Acceptable Use Agreement under Apache 2.0.
 * Please refer to the license file (LICENSE.md) included with this code.
 *
 */

#include "thlib.h"
#include "thal.h"

static void report_info( TCDef *tcdef );

/*------------------------------------------------------------------------------
 * FUNC   : th_timer_available
 *
 * DESC   : used to determine if the duration timer is available.
 *
 * RETURNS: TRUE if the target supports the duration timer.
 *          FALSE if not
 * ---------------------------------------------------------------------------*/

int th_timer_available( void )
{
	return TARGET_TIMER_AVAIL;
}

/*------------------------------------------------------------------------------
 * FUNC   : th_timer_is_intrusive
 *
 * DESC   : used to determine if the timer function is intrusive.
 *
 *          Intrusive usually means that operating and maintaining the timer
 *          has a run time overhead.  For example, using a 10ms interrupt
 *          to incriment a timer value is intrusive because the interrupt
 *          service routine takes CPU time.
 *
 *          If an intrusive target timer is used to measure benchmarks, then
 *          its effect must be measured and taken into account.
 *
 *          Some target timers may not be intrusive.  For example, a target
 *          with a built in real time clock can measure time without an
 *          interrupt service routine.
 *
 *          Another way to build a non-intrusive timer is to cascade
 *          counter/timer circuits to get very large divisors.  For example,
 *          with a 20mhz system clock two 16bit timers could be cascaded to
 *          measure durations up to 3 minutes and 34 seconds before rolling
 *          over.
 *
 *    NOTE: If the timer is not avaialable, then this function returns
 *          FALSE.
 *
 *
 * RETURNS: TRUE if the target's duration timer is intrusive
 *          FALSE if it is not
 * ---------------------------------------------------------------------------*/

int th_timer_is_intrusive( void )
{
	return TARGET_TIMER_INTRUSIVE;
}

/*------------------------------------------------------------------------------
 * FUNC   : th_ticks_per_sec
 *
 * DESC   : used to determine the number of timer ticks per second.
 *
 * RETURNS: The number of timer ticks per second returned by th_stop_timer()
 * ---------------------------------------------------------------------------*/

size_t th_ticks_per_sec( void )
{
	return al_ticks_per_sec();
}

/*------------------------------------------------------------------------------
 * FUNC   : th_tick_granularity
 *
 * DESC   : used to determine the granularity of timer ticks.
 *
 *          For example, the value returned by th_stop_timer() may be
 *          in milliseconds. In this case, th_ticks_pers_sec() would
 *          return 1000.  However, the timer interrupt may only occur
 *          once very 10ms.  So th_tick_granularity() would return 10.
 *
 *          However, on another system, th_ticks_sec() might return 10
 *          and th_tick_granularity() could return 1.  This means that each
 *          incriment of the value returned by th_stop_timer() is in 100ms
 *          intervals.
 *
 * RETURNS: the granularity of the value returned by th_stop_timer()
 * ---------------------------------------------------------------------------*/

size_t th_tick_granularity( void )
{
	return al_tick_granularity();
}

/*------------------------------------------------------------------------------
 * FUNC   : th_signal_start()
 *
 * DESC   : Signals the host system the test has started
 *
 *          Calling this function signals the host system that the test
 *          has actually started.  The host uses this signal to begin
 *          measuring the duration of the test (bench mark).
 *
 *          If a target based timer is avaialable, this this function will
 *          also start the target's timer.
 * ---------------------------------------------------------------------------*/

void th_signal_start( void )
{
al_signal_start();
}

/*------------------------------------------------------------------------------
 * FUNC   : th_signal_finished
 *
 * DESC   : Signals the host that the test is finished
 *
 *          This function is called to signal the host system that the
 *          currently executing test or benchmark is finished.  The host
 *          uses this signal to mark the stop time of the test and to
 *          measure the duration of the test.
 *
 * RETURNS: The duration of the test in 'ticks' as measured by the target's
 *          timer (if one is available).  If the target does not have a timer,
 *          or it is not supported by the TH, then this function returns
 *          TH_UNDEF_VALUE.
 *
 * NOTE   : There are intentionally no parameters for this function.  It is
 *          designed to have very low overhead. It causes a short message to
 *          be sent to the host indicating.  The results of the test are
 *          reported using another function.
 * ---------------------------------------------------------------------------*/

e_u32 th_signal_finished( void )
{
	return al_signal_finished();
}

/*------------------------------------------------------------------------------
 * FUNC   : th_exit
 *
 * DESC   : The benchmark code failed internally.
 *
 * RETURNS: Any error value supplied by the caller.
 * NOTE: differs from th_exit in that it returns exit code, and does
 *       not print unless windows example code.
 *       Add any signal code to aid debugging.
 * ----------------------------------------------------------------------------*/

void th_exit( int exit_code, const char *fmt, ... )
{
	va_list args;
	va_start( args, fmt );
	al_printf( fmt, args );
	va_end( args );
	al_exit(exit_code);
}

/*------------------------------------------------------------------------------
 * FUNC   : report_info
 *
 * DESC   : Reports the system info to the use or HCP
 * ---------------------------------------------------------------------------*/

static void report_info( TCDef *tcdef )
{
th_printf( ">>------------------------------------------------------------\n" );
th_printf( ">> EEMBC Component          : %s\n",EEMBC_TH_ID);
th_printf( ">> EEMBC Member Company     : %s\n",tcdef->member);
th_printf( ">> Target Processor         : %s\n",tcdef->processor);
th_printf( ">> Target Platform          : %s\n",tcdef->platform);
th_printf( ">> Target Timer Available   : %s\n",th_timer_available() ? "YES"   : "NO" );
th_printf( ">> Target Timer Intrusive   : %s\n",th_timer_is_intrusive() ? "YES" : "NO" );
th_printf( ">> Target Timer Rate        : %d\n",th_ticks_per_sec());
th_printf( ">> Target Timer Granularity : %d\n",th_tick_granularity());
#if		CRC_CHECK
th_printf( ">> Required Iterations      : %d\n",tcdef->rec_iterations);
#else
th_printf( ">> Recommended Iterations   : %d\n",tcdef->rec_iterations);
#endif
if (tcdef->iterations != tcdef->rec_iterations)
th_printf( ">> Programmed Iterations    : %d\n", tcdef->iterations );
th_printf( ">> Bench Mark               : %s\n",tcdef->desc);
}

/*------------------------------------------------------------------------------
 * FUNC   : th_report_results
 *
 * DESC   : Used to report a test's results after it is finished
 *
 *          This function is used to report the results of a test after
 *          it has run.  It differs from TH in that Expected CRC is reported, 
 *          and passed instead of a character string.
 * ---------------------------------------------------------------------------*/

int th_report_results(TCDef *tcdef, e_u16 Expected_CRC )
{
int	exit_code = Success;

/* Used to unload double from two vx results variables */ 
#if	VERIFY_FLOAT && FLOAT_SUPPORT
	typedef union {
		double	d;
		size_t	v[2];
	} d_union;
	d_union	dunion;
#endif

/* Standard Log file Print Section */
	report_info(tcdef);

/* Standard Results Section */

#if		CRC_CHECK
th_printf(  "--  Intrusive CRC     = %4x\n",tcdef->CRC);
#elif	NON_INTRUSIVE_CRC_CHECK
th_printf(  "--  Non-Intrusive CRC = %4x\n",tcdef->CRC);
#else
th_printf(  "--  No CRC check      = 0000\n"); 
#endif
th_printf(  "--  Iterations        = %5u\n", tcdef->iterations );
th_printf(  "--  Target Duration   = %5u\n", tcdef->duration );
#if		VERIFY_INT
th_printf(  "--  v1                = %d\n", tcdef->v1);
th_printf(  "--  v2                = %d\n", tcdef->v2);
th_printf(  "--  v3                = %d\n", tcdef->v3);
th_printf(  "--  v4                = %d\n", tcdef->v4);
#endif
#if		VERIFY_FLOAT && FLOAT_SUPPORT
dunion.v[0]	= tcdef->v1;
dunion.v[1] = tcdef->v2;
th_printf(  "--  v1v2              = %f\n", dunion.d);
dunion.v[0]	= tcdef->v3;
dunion.v[1] = tcdef->v4;
th_printf(  "--  v3v4              = %f\n", dunion.d);
#endif

#if		FLOAT_SUPPORT
   if (tcdef -> duration > 0)
      {
      double fduration;
      double fiterations;
      double ticks_per_sec;
      double its_per_sec;

      /* Ok, lets do this in floating point.... */

      fduration = tcdef -> duration;
	  fiterations = tcdef->iterations;
      ticks_per_sec = th_ticks_per_sec();

      its_per_sec = fiterations / ( fduration / ticks_per_sec );

      th_printf( "--  Iterations/Sec    = %12.3f\n", its_per_sec );
      th_printf( "--  Total Run Time    = %12.3fsec\n", fduration / ticks_per_sec );
      th_printf( "--  Time / Iter       = %18.9fsec\n", 1.0 / its_per_sec );
      }
#endif

   /* Failure Section */
#if		CRC_CHECK || NON_INTRUSIVE_CRC_CHECK
	if( tcdef->CRC != Expected_CRC ){
		th_printf("--  Failure: Actual CRC %x, Expected CRC %x\n",tcdef->CRC,Expected_CRC);
		exit_code = Failure;
	} 
#endif

	if (tcdef->iterations != tcdef->rec_iterations) {
		th_printf("--  Failure: Actual iterations %x, Expected iterations %x\n",tcdef->iterations,tcdef->rec_iterations);
		exit_code = Failure;
	}

if	(exit_code == SUCCESS )	th_printf( ">> DONE!\n" );
else						th_printf( ">> Failure: %d\n", exit_code );

	/* 
	 * user defined print information
	 * outside fixed standard log so automated scripts still work
	 */
	al_report_results();

/* Match TH Regular output */
	th_printf( ">> BM: %s\n", tcdef->desc );
	th_printf( ">> ID: %s\n\n", tcdef->eembc_bm_id );

	return	exit_code;
}

/*------------------------------------------------------------------------------
 * FUNC   : th_harness_poll
 *
 * DESC   : Gives the test harness some CPU time.
 *
 *          This function can be called during the execution of a test
 *          or benchmark to give the test harness some execution time so it
 *          can respond to commands from a host.
 *
 * CAVEAT : Only debug or test versions of true bench marks should call this
 *          function as it uses CPU time.
 *
 * RETURNS: TRUE if the test should keep running.
 *          FALSE if the stop message has been received form the host
 *          and the benchmark or test should stop.
 * NOTE:    Added to TH_Lite for application compatibility.
 * ---------------------------------------------------------------------------*/

int th_harness_poll( void )

   {
   return (TRUE);
   }
/*------------------------------------------------------------------------------
 * FUNC   : th_printf
 *
 * DESC   : The basic printf function. Does not need to be used in TH_Lite
 *
 * PARAMS : fmt - the classic printf format string, va's in stdarg.h
 * ---------------------------------------------------------------------------*/

int th_printf( const char *fmt, ... )

   {
   int rv;
   va_list args;
   va_start( args, fmt );
   rv = al_printf( fmt, args );
   va_end( args );
   return rv;
   }

/*------------------------------------------------------------------------------ * FUNC   : th_sprintf
 *
 * DESC   : The basic sprintf function.
 *
 * PARAMS : fmt - the classic printf format string
 * ---------------------------------------------------------------------------*/
int th_sprintf( char *str, const char *fmt, ... )

   {
   int rv;
   va_list args;
   va_start( args, fmt );
   rv = al_sprintf( str, fmt, args );
   va_end( args );
   return rv;
   }

/*==============================================================================
 *                  -- Funcational Layer Implemenation --
 * ===========================================================================*/

#if	!HAVE_ASSERT_H
const char *efn( const char *fn );

/*------------------------------------------------------------------------------
 * FUNC   : efn
 *
 * DESC   : returns a pointer to the file name portion of a path
 *
 *          Used to trim off drive letters and directory paths from file
 *          specs.
 *
 * NOTE    : this function assumes that both the '\' and '/' characters
 *           are directory path separators.  E.g. we handle both Microsoft
 *           and Unix style paths.
 *
 * RETURNS : a pointer to the file name
 * ---------------------------------------------------------------------------*/

const char *efn( const char *fn )
{
	const char *s;
	const char *end;

	/* Find the right most ':' or '/' or '\' in the string */

	for (end = s = fn; *s; s++)
	{
	if (*s == '/' || *s == '\\' || *s == ':')
		end = s + 1;
	}

	return end;
}

/*------------------------------------------------------------------------------
 * FUNC   : __assertfail
 *
 * DESC   : Called by the assert() macro when a condition fails
 *
 * NOTE   : Does not return!
 * ---------------------------------------------------------------------------*/

void __assertfail( const char *msg, const char *cond, const char *file, int line )

   {
   th_printf("message: %s\nCondition: %s\nFile: %s at %d\n",msg,cond,efn(file),line);
   al_exit( THE_FAILURE );
   }

/*------------------------------------------------------------------------------
 * FUNC   : __fatal
 *
 * DESC   : called when a fatal error is encountered
 * ---------------------------------------------------------------------------*/

void __fatal( const char *msg, const char *file, int line )

   {
   th_printf("message: %s\nFile: %s at %d\n",msg,efn(file),line);
   al_exit( THE_FAILURE );
   }
#endif

/*------------------------------------------------------------------------------
 * Malloc and Free Mapping
 *----------------------------------------------------------------------------*/

#if MAP_MALLOC_TO_TH
/*
 * If this is called by OS or crt0 thip will not be initialized, and
 * we need to give up. Attempt to return Error 8.
 */
void * malloc( size_t foo )
{
	if (thdef)
		return th_malloc( foo );
	else {
		exit(THE_OUT_OF_MEMORY);
	}
}

void free( void* ptr )
{
	if (thdef)
		th_free( ptr );
}
#endif

