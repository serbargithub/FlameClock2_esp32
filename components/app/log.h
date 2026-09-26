#ifndef _LOG_H
#define _LOG_H

//#define LOGGING_LEVEL0_ENABLED
//#define LOGGING_LEVEL1_ENABLED
#define DEBUG_MESSAGES_ENABLED

void LogMsg_lev0(char *text, ...);
void LogMsg_lev1(char *text, ...);
void DebugMsg(char *text, ...);

#endif //_LOG_H
