#ifndef SRC_METRONOME_H_
#define SRC_METRONOME_H_

//#include <sys/iofunc.h>
//#include <sys/dispatch.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//#include <string.h>
//#include <unistd.h>
//#include <errno.h>
//#include <unistd.h>
//#include <pthread.h>
//#include <sched.h>
//#include <math.h>
//#include <sys/types.h>
//#include <sys/netmgr.h>
//#include <sys/neutrino.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <string.h>

#define USAGE_MESSAGE1 "Usage: metronome <bpm> <ts-top> <ts-bottom>\n"
#define USAGE_MESSAGE2 "Metronome Resource Manager (ResMgr)\n\n\
 Usage: metronome <bpm> <ts-top> <ts-bottom>\n\n\
 API:\n\
 \tpause [1-9] - pause the metronome for 1-9 seconds\n\
 \tquit - quit the metronome\n\
 \tset <bpm> <ts-top> <ts-bottom> - set the metronome to <bpm> ts-top/ts-bottom\n\n\
 \tstart - start the metronome from stopped state\n\
 \tstop - stop the metronome; use ‘start’ to resume\n"
#define NUM_DEVICE 2

struct metronome{
	int bpm;
	int tst;
	int tsb;
} typedef metronome_t;

typedef struct ioattr {
	iofunc_attr_t attr;
	int device;
} ioattr_t;

char* DeviceName[NUM_DEVICE]={
	"/dev/local/metronome",
	"/dev/local/metronome-help",
};

typedef struct metro_ocb{
	iofunc_ocb_t ocb;
	char buffer[50];
}metro_ocb_t;

typedef union {
	struct _pulse pulse;
	char msg[255];
}my_message_t;

#define METRO_PULSE_CODE _PULSE_CODE_MINAVAIL         //0
#define PAUSE_PULSE_CODE (_PULSE_CODE_MINAVAIL +1) 	  //1
#define START_PULSE_CODE (_PULSE_CODE_MINAVAIL +2)    //2
#define STOP_PULSE_CODE  (_PULSE_CODE_MINAVAIL +3)    //3
#define QUIT_PULSE_CODE  (_PULSE_CODE_MINAVAIL +4)    //4
#define SET_PULSE_CODE   (_PULSE_CODE_MINAVAIL +5)    //5

#define START 0
#define STOPPED 1
#define PAUSED 2
#define PULSE 0
#define ERROR -1

struct DataTableRow{
	int tst;
	int tsb;
	int intervals;
	char pattern[16];
};
 /* Metronome Table */
struct DataTableRow t[] = {
		{2, 4, 4, "|1&2&"},
		{3, 4, 6, "|1&2&3&"},
		{4, 4, 8, "|1&2&3&4&"},
		{5, 4, 10, "|1&2&3&4-5-"},
		{3, 8, 6, "|1-2-3-"},
		{6, 8, 6, "|1&a2&a"},
		{9, 8, 9, "|1&a2&a3&a"},
		{12, 8, 12, "|1&a2&a3&a4&a"}
};

struct Timer_Properties{ /* Properties for the timer in the metronome calculating the beats */
	double bps; /* Beats per second */
	double measure; /* Beat per measure */
	double interval; /* Sec per interval */
	double nano_sec; /* nano seconds value */

}typedef timer_props_t;

#define DEVICES 2 /* Device amount */
#define METRONOME 0
#define METRONOME_HELP 1

//int io_read(resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb);/* POSIX I/O Function  @ Override*/
//int io_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb);/* POSIX I/O Function  @ Override*/
int io_open(resmgr_context_t *ctp, io_open_t *msg, RESMGR_HANDLE_T *handle,void *extra);/* POSIX Connections Function  @ Override*/
void metronome_thread(); /* Thread Work function */
void set_timer_props(metronome_t * Metronome);/* setting timer properties */
int search_idx_table(metronome_t * Metronome);/* search through Config table for metronome */
void stop_timer(struct itimerspec * itime, timer_t timer_id); /* Stops current timer */
void start_timer(struct itimerspec * itime, timer_t timer_id,metronome_t* Metronome);/* starts current timer */
metro_ocb_t * metro_ocb_calloc(resmgr_context_t *ctp, IOFUNC_ATTR_T *mtattr);
void metro_ocb_free(IOFUNC_OCB_T *mocb);

#endif /* SRC_METRONOME_H_ */
