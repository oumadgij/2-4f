#pragma once
#define	_USE_MATH_DEFINES
#define	RANKING_DATA	5
#define hold	65535/4

#include "DxLib.h"

//画面領域の大きさ
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int APPLE_MAX = 10;

const int TIMELIMIT = 1800;

extern int g_TimeLimit;		//スタート時間
extern int g_WaitTime;		//待ち時間
extern int g_GameState;		// ゲームステータス

extern XINPUT_STATE g_OldKey;			//前回の入力キー
extern XINPUT_STATE g_NowKey;			//今回の入力キー

extern int g_Score;			//スコア
extern int g_AppleCount[3];		//敵カウント

extern int FontHandle1;
extern int FontHandle2;
extern int FontHandle3;

extern int g_MainBGM;          //ゲームメインBGM
extern int g_CatchSE;			//A,B,Cのリンゴ取得時のSE
extern int g_PoisonSE;			//Dのリンゴ取得時のSE