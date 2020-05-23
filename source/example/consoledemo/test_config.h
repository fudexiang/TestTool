#pragma once
#ifndef _TEST_CONFIG_H_
#define _TEST_CONFIG_H_

#define END_APP_BY_Q
//#define PLAY_MUSIC
//#define REGISTER_TOOL
#define AUDIO_PLAYER_CAPTURE
//#define TEST_1
//#define TEST_2
//#define TEST_3
//#define TEST_10
//#define TEST_11


#ifdef PLAY_MUSIC
extern void Audio_Demo_Test(Threads_Control_t* pControl);
#endif
#ifdef REGISTER_TOOL
extern void Audio_Register_Tool_Test(Threads_Control_t* pControl);
#endif

#ifdef TEST_1
extern void Mytest1(Threads_Control_t* p);
#endif

#ifdef TEST_2
extern void Mytest2(Threads_Control_t* p);
#endif

#ifdef TEST_3
extern void Mytest3(Threads_Control_t* p);
#endif

#ifdef TEST_10
extern void MyWebtest(void);
#endif

#ifdef TEST_11
extern void Mytest11(void);
#endif

#ifdef AUDIO_PLAYER_CAPTURE
extern void WasTest(Threads_Control_t* gpConfig);
#endif

#endif