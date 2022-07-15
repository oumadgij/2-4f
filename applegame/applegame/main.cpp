﻿#include"DxLib.h"
#include"common.h"
#include"Player.h"
#include"Apple.h"
#include <math.h>

//変数の宣言
XINPUT_STATE g_OldKey;			//前回の入力キー
XINPUT_STATE g_NowKey;			//今回の入力キー
//int g_KeyFlg;			//入力キー情報

int g_GameState = 0;	//ゲームモード
int g_WaitTime;		//待ち時間
int g_TimeLimit;		//スタート時間

int g_Score;			//スコア
int g_AppleCount[3] = { 0,0,0 };	//リンゴを取った数

int SpawnX[7] = { 0,0,0,0,0,0,0 };			//リンゴ生成位置(0:未生成 1:生成済み)

int g_TitleImage;		//画像用変数
int g_Menu, g_Leaf;		//メニュー画像変数
int g_RankingImage;		//ランキング画像用変数
int g_Item[2];			//アイテム画像変数
int g_EndImage;			//エンド画面用変数
int g_Teki[4];			//敵画像変数
int g_StageImage;		//ステージ画像変数
int g_PlayerImage[2];	//キャラ画像
int g_PauseImage;		//ポーズ画像
int g_TimeupImage;	
int g_KanbanImage;
int g_HelpImage;
int g_BImage[3];

int g_TitleBGM;         //タイトルBGM
int g_MainBGM;          //ゲームメインBGM
int g_CatchSE;			//A,B,Cのリンゴ取得時のSE
int g_PoisonSE;			//Dのリンゴ取得時のSE
int g_PauseSE;			//ポーズSE
int g_FallSE;			//リンゴ出現SE
int g_cursorSE;			//カーソルSE
int g_selectSE;			//セレクトSE
int g_SelectGameSE;     //ゲームスタートが押されたときのSE(Titleの[はじめる]・Rankingの[Aボタンで開始])
int g_deleteSE;         //入力した文字を消す時のSE
int g_ErrorSE;			//一文字も入力されずにENDを押された時のSE
int g_TimeUPSE;         //タイムアップ時のSE

int FontHandle1;
int FontHandle2;
int FontHandle3;
int FontHandle4;

double NextTime;		//フレーム毎の経過時間

//ランキングデータ(構造体)
struct RankingData {
	int no;
	char name[11];
	long score;
};
char PlayerName[11];

//ランキングデータ変数宣言
struct RankingData	g_Ranking[RANKING_DATA];

//関数のプロトタイプ宣言
void GameInit(void);	//ゲーム初期化処理
void GameMain(void);	//ゲームメイン処理

void DrawGameTitle(void);	//タイトル描画処理
void DrawGameOver(void);	//ゲームオーバー画面描画処理
void DrawEnd(void);			//ゲームエンド描画処理
void DrawHelp(void);		//ゲームヘルプ描画処理

int CheckApple(void);		//リンゴの生成可能数チェック
int SpawnAppleX(void);		//出現するリンゴのX座標を決定

void BackScrool();		//背景画像スクロール処理

//int HitBoxPlayer(PLAYER *p, ENEMY *e);	//当たり判定

void DrawRanking(void);		//ランキング描画処理
void InputRanking(void);	//ランキング入力
void SortRanking(void);		//ランキング並び替え
int  SaveRanking(void);		//ランキングデータの保存
int  ReadRanking(void);		//ランキングデータ読み込み

int LoadImages();			//画像読み込み
int LoadSounds();			//音源読み込み

void CheckPauseKey();	//ポーズ画面

//プログラムの開始
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) {

	SetMainWindowText("りんご落とし");		//タイトルを設定

	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)return -1;

	SetDrawScreen(DX_SCREEN_BACK);

	if (LoadImages() == -1)return -1;

	if (LoadSounds() == -1) return -1;

	if (ReadRanking() == -1)return -1;

	FontHandle1 = CreateFontToHandle("HGS創英角ﾎﾟｯﾌﾟ体", 55, 3, DX_FONTTYPE_NORMAL);
	FontHandle2 = CreateFontToHandle("HGS創英角ﾎﾟｯﾌﾟ体", 27, 3, DX_FONTTYPE_NORMAL);
	FontHandle3 = CreateFontToHandle("HGS創英角ﾎﾟｯﾌﾟ体", 20, 3, DX_FONTTYPE_NORMAL);
	FontHandle4 = CreateFontToHandle("HGS創英角ﾎﾟｯﾌﾟ体", 36, 3, DX_FONTTYPE_NORMAL);

	while (ProcessMessage() == 0 && g_GameState != 99 && !(g_NowKey.Buttons[XINPUT_BUTTON_BACK])) {
	
		g_OldKey = g_NowKey;
		GetJoypadXInputState(DX_INPUT_PAD1, &g_NowKey);

		ClearDrawScreen();	//画面の初期化

		switch (g_GameState) {
		case 0:
			DrawGameTitle();	//ゲームタイトル描画
			break;

		case 1:
			GameInit();			//ゲーム初期処理
			break;

		case 2:
			DrawRanking();		//ランキング描画
			break;

		case 3:
			DrawHelp();			//ゲームヘルプ描画
			break;

		case 4:
			DrawEnd();			//ゲームエンド描画
			break;

		case 5:
			GameMain();			//ゲームメイン処理
			break;

		case 6:
			DrawGameOver();		//ゲームオーバー描画
			break;

		case 7:
			InputRanking();		//ランキング入力
			break;
		}

		NextTime += 16.66;								//1フレームごとに 1000/60 = 約16.66ミリ秒かかる			
		if (NextTime > GetNowCount()) {					//経過フレーム*16.66秒経過した？
			WaitTimer((int)NextTime - GetNowCount());	//してないなら経過するまで待つ
		}
		ScreenFlip();			//裏画面の内容を表画面に反映
	}
	DeleteFontToHandle(FontHandle1);
	DeleteFontToHandle(FontHandle2);
	DeleteFontToHandle(FontHandle3);
	DeleteFontToHandle(FontHandle4);

	DxLib_End();				//DXライブラリ仕様の終了処理

	return 0;					//ソフトの終了

}

//ゲームタイトル表示
void DrawGameTitle(void) {
	static int MenuNo = 0;

	//ゲームメインBGMが再生されていたら再生を止める
	if (CheckSoundMem(g_MainBGM) == 1)   StopSoundMem(g_MainBGM);
	//タイトルBGMの再生
	if (CheckSoundMem(g_TitleBGM) == 0) PlaySoundMem(g_TitleBGM, DX_PLAYTYPE_LOOP);

	if (MenuNo > 3 || MenuNo < 0) MenuNo = 0;
	
	if (g_OldKey.ThumbLY <= hold && g_NowKey.ThumbLY > hold) {
		if (--MenuNo < 0)MenuNo = 3;
		PlaySoundMem(g_cursorSE, DX_PLAYTYPE_BACK, TRUE);
	}
	if (g_OldKey.ThumbLY >= -hold && g_NowKey.ThumbLY < -hold) {
		if (++MenuNo > 3)MenuNo = 0;
		PlaySoundMem(g_cursorSE, DX_PLAYTYPE_BACK, TRUE);
	}

	//Zキーでメニュー選択
	if (!(g_OldKey.Buttons[XINPUT_BUTTON_A]) && g_NowKey.Buttons[XINPUT_BUTTON_A]) {
		if(MenuNo != 0)  PlaySoundMem(g_selectSE, DX_PLAYTYPE_BACK, TRUE);    //[はじめる]以外でAボタンを押されたとき
		else {
			PlaySoundMem(g_SelectGameSE, DX_PLAYTYPE_BACK, TRUE);             //[はじめる]でAボタンを押されたとき
		}

		g_GameState = MenuNo + 1;
		MenuNo = 0;
	}

	//タイトル画像表示
	DrawGraph(0, 0, g_TitleImage, FALSE);

	//メニュー
	DrawGraph(390, 270, g_Menu, TRUE);

	//メニューカーソル
	DrawGraph(325, 259 + MenuNo * 40, g_Leaf, TRUE);
}

//ゲーム初期処理
void GameInit(void) {

	//時間初期化
	g_WaitTime = 0;

	//スコア初期化
	g_Score = 0;

	//敵の初期設定
	g_AppleCount[0] = 0;
	g_AppleCount[1] = 0;
	g_AppleCount[2] = 0;

	//プレイヤー初期設定
	player.Init();

	//敵の初期設定
	for (int i = 0; i < APPLE_MAX; i++) {
		apple[i].flg = FALSE;
	}

	for (int i = 0; i < 10; i++) {
		PlayerName[i] = NULL;
	}
	PlayerName[10] = '\0';

	//制限時間をセット
	g_TimeLimit = 60 * (TIMELIMIT + 1);

	//ゲームメイン処理へ
	g_GameState = 5;

	StopSoundMem(g_TitleBGM);
	PlaySoundMem(g_MainBGM, DX_PLAYTYPE_LOOP, TRUE);    //メインBGMの再生
}

//ゲームランキング画面表示
void DrawRanking(void) {

	//Bでメニューに戻る
	if (!(g_OldKey.Buttons[XINPUT_BUTTON_B]) && g_NowKey.Buttons[XINPUT_BUTTON_B]) {
		g_GameState = 0;
		PlaySoundMem(g_selectSE, DX_PLAYTYPE_BACK, TRUE);
	}

	//ランキング画面表示
	DrawGraph(0, 0, g_RankingImage, FALSE);
	//DrawGraph(120, 120, g_KanbanImage, TRUE);

	//ランキング一覧を表示
	/*SetFontSize(30);*/
	for (int i = 0; i < RANKING_DATA; i++) {
		DrawFormatStringToHandle(90, 150 + i * 50, 0xffffff, FontHandle4, "%2d", g_Ranking[i].no);
		DrawFormatStringToHandle(90 + 36 * 2, 150 + i * 50, 0xffffff, FontHandle4, "%-10s", g_Ranking[i].name);
		DrawFormatStringToHandle(90 + 36 * 3 + 180, 150 + i * 50, 0xffffff, FontHandle4, "%10d", g_Ranking[i].score);
	}

	DrawGraph(185, 418, g_BImage[0], TRUE);

}

//ゲームヘルプ描画
void DrawHelp(void) {
	
	if (!(g_OldKey.Buttons[XINPUT_BUTTON_A]) && g_NowKey.Buttons[XINPUT_BUTTON_A]) {
		//Aでゲームスタート
		g_GameState = 1;
		PlaySoundMem(g_SelectGameSE, DX_PLAYTYPE_BACK, TRUE);
	}
	else if (!(g_OldKey.Buttons[XINPUT_BUTTON_B]) && g_NowKey.Buttons[XINPUT_BUTTON_B]) {
		//Bでメニューに戻る
		g_GameState = 0;
		PlaySoundMem(g_selectSE, DX_PLAYTYPE_BACK, TRUE);
	}

	//タイトル画像表示
	DrawGraph(0, 0, g_HelpImage, FALSE);

}

//ゲームエンド描画
void DrawEnd(void) {
	static int EndTime = 180;

	//エンド画面表示
	DrawGraph(0, 0, g_EndImage, FALSE);

	//タイム加算処理&終了(3秒)
	if (--EndTime < 0)g_GameState = 99;

}

//ゲームメイン
void GameMain(void) {
	static int restapple = CheckApple();	//リンゴ生成可能数

	BackScrool();

	for (int i = 0; i < APPLE_MAX; i++) {
		apple[i].EnemyControl();	//リンゴ制御
		if (player.GetD() <= 0) {		//りんごDの効果中ではない？
			player.AppleColision(i);	//効果中でないならりんごの当たり判定を取る
		}
	}
	if (g_WaitTime++ % 25 == 0&& g_WaitTime % 25 != 0) {	//25fごとにリンゴ出現
		for (int i = 0; i < (restapple + 1) / 2; i++) {		//リンゴの生成数チェック(生成可能数/2)
			for (int j = i; j < APPLE_MAX; j++) {			
				if (apple[j].flg == FALSE) {			//apple[j]のりんごが出現中か(フラグがTRUEか)
					apple[j].Spawn(SpawnAppleX());		//非出現(フラグがFALSE)ならX座標を決定しリンゴを出現させる
					PlaySoundMem(g_FallSE, DX_PLAYTYPE_BACK, TRUE);
					break;						//ループを抜ける
				}
			}
		}
		for (int k = 0; k < 7; k++) {
			SpawnX[k] = 0;				//リンゴ生成位置をリセット
		}
		restapple = CheckApple();
	}

	player.PlayerControl();	//プレイヤー制御
	CheckPauseKey();	//ポーズ画面

	//計測時間を過ぎたらゲームオーバー
	if (g_TimeLimit-- <= 60) {
		/*g_TimeLimit = 0;*/
		g_GameState = 6;
		PlaySoundMem(g_TimeUPSE, DX_PLAYTYPE_BACK, TRUE);
	}

}

//リンゴの生成可能数チェック
int CheckApple(void) {
	int rest = 0;

	for (int i = 0; i < APPLE_MAX; i++) {
		if (apple[i].flg == FALSE) rest++;
	}
	return rest;
}

//出現するリンゴのX座標を決定
int SpawnAppleX(void) {
	int x = 0;

	do  {
		x = GetRand(6);			//X座標をランダム選択
	} while (SpawnX[x] != 0);	//その位置にすでに生成されている？

	SpawnX[x] = 1;
	return x;
}

//背景画像スクロール処理
void BackScrool() {

	//ステージ画像表示

	//描画可能エリアを設定
	SetDrawArea(0, 0, 500, 480);

	DrawGraph(0, 0, g_StageImage, FALSE);

	//エリアを戻す
	SetDrawArea(0, 0, 640, 480);

	//スコア等表示領域
	DrawBox(500, 0, 640, 480, 0x009900, TRUE);

	DrawFormatStringToHandle(511, 40, 0xfdeca6,FontHandle3, "のこりじかん");
	DrawFormatStringToHandle(535, 70, 0xffffff, FontHandle1, "%02d", g_TimeLimit / 60); //制限時間の描画

	//スコアの描画
	DrawFormatStringToHandle(533, 170, 0xfdeca6, FontHandle3, "てんすう");
	DrawFormatStringToHandle(518, 200, 0xFFFFFF, FontHandle2, "%06d", g_Score);

	//獲得したりんごの個数を描画
	DrawFormatStringToHandle(520, 275, 0xfdeca6, FontHandle3, "とったかず");
	DrawRotaGraph(535, 325, 0.9f, 0, g_Teki[2], TRUE, FALSE);
	DrawRotaGraph(535, 375, 0.9f, 0, g_Teki[1], TRUE, FALSE);
	DrawRotaGraph(535, 425, 0.9f, 0, g_Teki[0], TRUE, FALSE);

	DrawFormatStringToHandle(558, 317, 0xFFFFFF, FontHandle3, " ×  %2d", g_AppleCount[2]);
	DrawFormatStringToHandle(558, 367, 0xFFFFFF, FontHandle3, " ×  %2d", g_AppleCount[1]);
	DrawFormatStringToHandle(558, 417, 0xFFFFFF, FontHandle3, " ×  %2d", g_AppleCount[0]);

}

//敵との当たり判定
//int HitBoxPlayer(player *p, apple *e) {
//	//x,yは中心座標
//	int sx1 = p->x - (p->w / 2) + 5;
//	int sy1 = p->y - (p->h / 2) + 5;
//	int sx2 = sx1 + p->w - 10;
//	int sy2 = sy1 + p->h - 10;
//
//	int dx1 = e->x - (e->w / 2) + 5;
//	int dy1 = e->y - (e->h / 2) + 5;
//	int dx2 = dx1 + e->w - 10;
//	int dy2 = dy1 + e->h - 10;
//
//	//矩形が重なれば当たり
//	if (sx1 < dx2 && dx1 < sx2 && sy1 < dy2 && dy1 < sy2) {
//		return TRUE;
//	}
//	return FALSE;
//}
//


//ゲームオーバー画面描画
void DrawGameOver(void) {

	BackScrool();

	//スペースキーでメニューに戻る
	if (!(g_OldKey.Buttons[XINPUT_BUTTON_B]) && g_NowKey.Buttons[XINPUT_BUTTON_B]) {
		if (g_Score > g_Ranking[RANKING_DATA - 1].score) {
			g_GameState = 7;
		}
		else {
			g_GameState = 0;
		}
	}

	DrawGraph(0, 0, g_TimeupImage, TRUE);

	if (g_Score > g_Ranking[RANKING_DATA - 1].score) {
		DrawGraph(115, 418, g_BImage[1], TRUE);
	}
	else {
		DrawGraph(115, 418, g_BImage[0], TRUE);
	}
}

 //ランキング入力処理
void InputRanking(void)
{
	//文字配列
	static char NAME[27] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
	static char name[27] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
	static char number[10] = { '0','1','2','3','4','5','6','7','8','9' };

	//アイコン
	static int IconX = 0;
	static int IconY = 0;

	//文字入力位置
	static int Input = 0;

	while (PlayerName[Input] != NULL && Input < 10) {
		Input++;
	}

	//ランキング画像表示
	DrawGraph(0, 0, g_RankingImage, FALSE);

	//ボタン操作説明画像表示
	DrawGraph(0, 415, g_BImage[2], TRUE);
	
	// 名前入力指示文字列の描画
	for (int i = 0; i < 13; i++) {
		DrawFormatStringToHandle(45 * i + 40, 195, 0x000000, FontHandle4, "%c", NAME[i]);
		DrawFormatStringToHandle(45 * i + 40, 235, 0x000000, FontHandle4, "%c", NAME[i + 13]);
		DrawFormatStringToHandle(45 * i + 40, 275, 0x000000, FontHandle4, "%c", name[i]);
		DrawFormatStringToHandle(45 * i + 40, 315, 0x000000, FontHandle4, "%c", name[i + 13]);
		if (i < 10)DrawFormatStringToHandle((45 * i) + 40, 355, 0x000000, FontHandle4, "%c", number[i]);
	}
	DrawFormatStringToHandle(45 * 11 + 40, 355, 0x000000, FontHandle4, "END");

	//カーソル描画
	if (IconX > 9 && IconY > 3) {	//カーソルがENDに移動した?
		DrawBox(45 * 11 + 40 - 10, 40 * 4 + 195 - 5, 45 * 12 + 40 + 30, 40 * 4 + 200 + 36, 0x00ffff, FALSE);	//したならカーソルをENDの上に
		if (g_OldKey.ThumbLY >= -hold && g_NowKey.ThumbLY < -hold)	IconY = 0;
		if (g_OldKey.ThumbLY <= hold && g_NowKey.ThumbLY > hold)	IconY = 3;
		if (g_OldKey.ThumbLX >= -hold && g_NowKey.ThumbLX < -hold)	IconX = 9;
		if (g_OldKey.ThumbLX <= hold && g_NowKey.ThumbLX > hold)	IconX = 0;
	}
	else {																										//してなければ文字の上に
		DrawBox(45 * IconX + 40 - 5, 40 * IconY + 195 - 5, 45 * IconX + 40 + 31, 40 * IconY + 200 + 36, 0x00ffff, FALSE);
		if (g_OldKey.ThumbLY >= -hold && g_NowKey.ThumbLY < -hold) {
			if (++IconY > 4)IconY = 0;
		}
		if (g_OldKey.ThumbLY <= hold && g_NowKey.ThumbLY > hold) {
			if (--IconY < 0)IconY = 4;
		}
		if (g_OldKey.ThumbLX <= hold && g_NowKey.ThumbLX > hold) {
			if (++IconX > 12)IconX = 0;
		}
		if (g_OldKey.ThumbLX >= -hold && g_NowKey.ThumbLX < -hold) {
			if (--IconX < 0)IconX = 12;
		}
	}
	if (PlayerName[9] != NULL) {		//最大まで入力されているならアイコンをENDに固定
		IconX = 12;
		IconY = 4;
	}

	//カーソルが移動するとSEを流す
	if ((g_OldKey.ThumbLY >= -hold && g_NowKey.ThumbLY < -hold) || (g_OldKey.ThumbLY <= hold && g_NowKey.ThumbLY > hold) || (g_OldKey.ThumbLX <= hold && g_NowKey.ThumbLX > hold) || (g_OldKey.ThumbLX >= -hold && g_NowKey.ThumbLX < -hold)) {
		PlaySoundMem(g_cursorSE, DX_PLAYTYPE_BACK, TRUE);
	}

	//入力
	if (!(g_OldKey.Buttons[XINPUT_BUTTON_A]) && g_NowKey.Buttons[XINPUT_BUTTON_A]) {	//Aが押された

		if (IconX <= 9 || IconY <= 3) PlaySoundMem(g_selectSE, DX_PLAYTYPE_BACK, TRUE);      //カーソルが文字・数字の上ならg_selectSEを再生							
	

		if (IconY <= 1)PlayerName[Input] = NAME[IconX + (IconY * 13)];				//カーソルが大文字の上なら大文字の入力
		else if (IconY <= 3)PlayerName[Input] = name[IconX + ((IconY - 2) * 13)];	//　　　　　小文字の上なら小文字の入力
		else if (IconX <= 9)PlayerName[Input] = number[IconX];						//			　数字の上なら　数字の入力
		else if (PlayerName[0] != NULL) {			//どれでもない（ENDの上）かつ　1文字以上入力されているなら　　入力終了
			for (int i = 0; i < 11; i++) {
				g_Ranking[4].name[i] = PlayerName[i];
			}
			g_Ranking[4].score = g_Score;
			SortRanking();		// ランキング並べ替え
			SaveRanking();		// ランキングデータの保存
			IconX = 0;
			IconY = 0;
			Input = 0;
			g_GameState = 2;		// ゲームモードの変更
			PlaySoundMem(g_SelectGameSE, DX_PLAYTYPE_BACK, TRUE);      //g_SelectGameSEを再生
		}
		else {			//ENDの上で一文字以上入力されていない時ErrorSEを再生する
			PlaySoundMem(g_ErrorSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (!(g_OldKey.Buttons[XINPUT_BUTTON_B]) && g_NowKey.Buttons[XINPUT_BUTTON_B] && Input >= 1) {
		PlayerName[Input - 1] = NULL;
		Input--;

		PlaySoundMem(g_deleteSE, DX_PLAYTYPE_BACK, TRUE);
	}

	for (int i = 0; i < 10; i++) {
		DrawBox(25 * i + 40, 165, 25 * i + 60, 170, 0xffffff, TRUE);
		DrawFormatStringToHandle(25 * i + 40, 130, 0xff0000, FontHandle4, "%c", PlayerName[i]);
	}

}

 //ランキング並べ替え
void SortRanking(void)
{
	int i, j;
	RankingData work;

	// 選択法ソート
	for (i = 0; i < 4; i++) {
		for (j = i + 1; j < RANKING_DATA; j++) {
			if (g_Ranking[i].score <= g_Ranking[j].score) {
				work = g_Ranking[i];
				g_Ranking[i] = g_Ranking[j];
				g_Ranking[j] = work;
			}
		}
	}

	// 順位付け
	for (i = 0; i < 5; i++) {
		g_Ranking[i].no = 1;
	}
	// 得点が同じ場合は、同じ順位とする
	// 同順位があった場合の次の順位はデータ個数が加算された順位とする
	for (i = 0; i < 4; i++) {
		for (j = i + 1; j < 5; j++) {
			if (g_Ranking[i].score > g_Ranking[j].score) {
				g_Ranking[j].no++;
			}
		}
	}
}

 //ランキングデータの保存
int  SaveRanking(void)
{
	FILE* fp;
#pragma warning(disable:4996)

	// ファイルオープン
	if ((fp = fopen("dat/rankingdata.txt", "w")) == NULL) {
		/* エラー処理 */
		printf("Ranking Data Error\n");
		return -1;
	}

	// ランキングデータ分配列データを書き込む
	for (int i = 0; i < RANKING_DATA; i++) {
		fprintf(fp, "%2d %10s %10d\n", g_Ranking[i].no, g_Ranking[i].name, g_Ranking[i].score);
	}

	//ファイルクローズ
	fclose(fp);

	return 0;

}

 //ランキングデータ読み込み
int ReadRanking(void)
{
	FILE* fp;
#pragma warning(disable:4996)

	//ファイルオープン
	if ((fp = fopen("dat/rankingdata.txt", "r")) == NULL) {
		//エラー処理
		printf("Ranking Data Error\n");
		return -1;
	}

	//ランキングデータ配分列データを読み込む
	for (int i = 0; i < RANKING_DATA; i++) {
		fscanf(fp, "%2d %10s %10d", &g_Ranking[i].no, g_Ranking[i].name, &g_Ranking[i].score);
	}

	//ファイルクローズ
	fclose(fp);

	return 0;
}

int LoadImages() {

	//タイトル
	if ((g_TitleImage = LoadGraph("images/Title.png")) == -1)return -1;

	//メニュー
	if ((g_Menu = LoadGraph("images/menu.png")) == -1)return -1;
	if ((g_Leaf = LoadGraph("images/leaf.png")) == -1)return -1;

	//ランキング
	if ((g_RankingImage = LoadGraph("images/Ranking.png")) == -1)return -1;

	//ステージ背景
	if ((g_StageImage = LoadGraph("images/back.png")) == -1)return -1;

	//プレイヤー
	if ((g_PlayerImage[0] = LoadGraph("images/player0.png")) == -1)return -1;
	if ((g_PlayerImage[1] = LoadGraph("images/player1.png")) == -1)return -1;

	//りんご
	if (LoadDivGraph("images/apple.png", 4, 4, 1, 40, 40, g_Teki) == -1)return -1;

	//ポーズ
	if ((g_PauseImage = LoadGraph("images/pause.png")) == -1)return -1;

	//時間切れ
	if ((g_TimeupImage = LoadGraph("images/timeup.png")) == -1)return -1;

	//ランキング用看板
	if ((g_KanbanImage = LoadGraph("images/kanban.png")) == -1)return -1;

	//help
	if ((g_HelpImage = LoadGraph("images/help.png")) == -1)return -1;
	if ((g_BImage[0] = LoadGraph("images/BbuttonTitle.png")) == -1)return -1;
	if ((g_BImage[1] = LoadGraph("images/BbuttonRank.png")) == -1)return -1;
	if ((g_BImage[2] = LoadGraph("images/Rankingbutton.png")) == -1)return -1;

	//エンディング
	if ((g_EndImage = LoadGraph("images/end.png")) == -1)return -1;

	return 0;
}

//ポーズ画面
void CheckPauseKey(void) {
	if (!(g_OldKey.Buttons[XINPUT_BUTTON_START]) && g_NowKey.Buttons[XINPUT_BUTTON_START])		//指定キーでflgを1
	{
		StopSoundMem(g_MainBGM);  //メインBGMを止める
		PlaySoundMem(g_PauseSE, DX_PLAYTYPE_BACK, TRUE);  //ポーズSEを再生
		int flg = 1;

		while (ProcessMessage() == 0 && flg)
		{
			g_OldKey = g_NowKey;
			GetJoypadXInputState(DX_INPUT_KEY_PAD1, &g_NowKey);

			DrawGraph(0, 0, g_PauseImage, TRUE);

			if (!(g_OldKey.Buttons[XINPUT_BUTTON_START]) && g_NowKey.Buttons[XINPUT_BUTTON_START]) {
				flg = 0;		//指定キーでFlgを0
				PlaySoundMem(g_PauseSE, DX_PLAYTYPE_BACK, TRUE);  //ポーズSEを再生
				PlaySoundMem(g_MainBGM, DX_PLAYTYPE_LOOP, FALSE);
			}
			if (!(g_OldKey.Buttons[XINPUT_BUTTON_START]) && g_NowKey.Buttons[XINPUT_BUTTON_BACK]) {
				DxLib_End();
			}

			ScreenFlip();			//裏画面の内容を表画面に反映

		}
	}
}

int LoadSounds()
{
	//ゲームメインBGM
	if ((g_MainBGM = LoadSoundMem("sounds/MainBGM.wav")) == -1) return -1;
	//タイトルBGM
	if ((g_TitleBGM = LoadSoundMem("sounds/TitleBGM.wav")) == -1) return -1;

	//A～Cのリンゴ取得時のSE
	if ((g_CatchSE = LoadSoundMem("sounds/catchSE.wav")) == -1) return-1;
	//Dのリンゴ取得時のSE
	if ((g_PoisonSE = LoadSoundMem("sounds/poisonSE.wav")) == -1) return -1;
	//リンゴ出現時のSE
	if ((g_FallSE = LoadSoundMem("sounds/FallSE.wav")) == -1)return -1;
	//ポーズ画面に移行した時のSE
	if ((g_PauseSE = LoadSoundMem("sounds/PoseSE.wav")) == -1) return -1;
	//タイムアップ時のSE
	if ((g_TimeUPSE = LoadSoundMem("sounds/TitmeUPSE.wav")) == -1) return -1;

	//カーソル移動SE
	if ((g_cursorSE = LoadSoundMem("sounds/cursorSE.wav")) == -1)return -1;
	//決定時のSE
	if ((g_selectSE = LoadSoundMem("sounds/selectSE.wav")) == -1)return -1;
	//ゲームスタートが押された時のSE
	if ((g_SelectGameSE = LoadSoundMem("sounds/SelectGameSE.wav")) == -1) return -1;

	//入力した文字を削除する時のSE
	if ((g_deleteSE = LoadSoundMem("sounds/deleteSE.wav")) == -1) return -1;
	//文字が入力されずにENDを押された時のSE
	if ((g_ErrorSE = LoadSoundMem("sounds/ErrorSE.wav")) == -1) return -1;

	//音量調整
	ChangeVolumeSoundMem(250, g_cursorSE);   //カーソル移動SE
	ChangeVolumeSoundMem(400, g_PoisonSE);   //リンゴD取得時のSE
	ChangeVolumeSoundMem(170, g_FallSE);	 //リンゴ落下SE
	ChangeVolumeSoundMem(300, g_ErrorSE);    //エラーSE

	return 0;
}