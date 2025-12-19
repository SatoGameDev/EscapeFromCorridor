#pragma once
#include <DxLib.h>
#include <string>
#include "Base/SceneBase.h"
#include "../Application.h"
#include "../Common/Vector2.h"



class Title : public SceneBase
{
public:
	//--------------
	// テキスト関連
	//--------------
	// ゲームスタート
	static constexpr float GAMESTART_TEXT_X = 1000.0f;							// 左上 X座標
	static constexpr float GAMESTART_TEXT_Y = 550.0f;							//		Y座標
	static constexpr float GAMESTART_TEXT_X_END = GAMESTART_TEXT_X + 350.0f;	// 右下 X座標
	static constexpr float GAMESTART_TEXT_Y_END = GAMESTART_TEXT_Y + 60.0f;		//		Y座標

	// 操作方法
	static constexpr float KEYHELP_TEXT_X = 1000.0f;							// 左上 X座標
	static constexpr float KEYHELP_TEXT_Y = 620.0f;								//		Y座標
	static constexpr float KEYHELP_TEXT_X_END = KEYHELP_TEXT_X + 350.0f;		// 右下 X座標
	static constexpr float KEYHELP_TEXT_Y_END = KEYHELP_TEXT_Y + 60.0f;			//		Y座標

	// ゲーム終了
	static constexpr float GAMEEND_TEXT_X = 1000.0f;							// 左上 X座標
	static constexpr float GAMEEND_TEXT_Y = 690.0f;								//		Y座標
	static constexpr float GAMEEND_TEXT_X_END = GAMEEND_TEXT_X + 350.0f;		// 右下 X座標
	static constexpr float GAMEEND_TEXT_Y_END = GAMEEND_TEXT_Y + 60.0f;			//		Y座標

	// メニュー項目の数
	static constexpr int MENU_NUM = 3;

	// テキストの構造体
	struct text
	{
		Vector2 textLeftUpPos_;		// 文字の左上の座標
		Vector2 textRightDownPos_;	// 文字の右上の座標
		int choicePic;			    // メニュー選択状態の画像
		int notChoicePic;			// メニュー非選択状態の画像
		int clickPic;				// メニュー決定時
		bool textFlg_;				// 文字選択フラグ
	};

	Title(void);					// コンストラクタ
	~Title(void) override;			// デストラクタ

	// 初期処理
	void Init(void) override;		// 初期化
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void Release(void) override;	// 解放

private:
	// 構造体の配列
	struct text list[MENU_NUM];

	// 文字情報配列
	Vector2 stringLeftUpPos	  [MENU_NUM];	// 左上座標
	Vector2 stringRightDownPos[MENU_NUM];	// 右下座標
	int		menuChoice		  [MENU_NUM];	// メニュー選択状態の画像
	int		menuNotChoice	  [MENU_NUM];	// メニュー非選択状態の画像
	int     menuClick		  [MENU_NUM];	// メニュー決定状態の画像
	bool    clickFlg		  [MENU_NUM];	// メニュー決定フラグ

	// 画像のハンドル
	int haikei_;		// 背景

	int gamestart_choice;
	int gamestart_notchoice;
	int gamestart_click;

	int keyhelp_choice;
	int keyhelp_notchoice;
	int keyhelp_click;

	int gameend_choice;
	int gameend_notchoice;
	int gameend_click;

	// SEが再生中かどうか示すフラグ
	bool isSEPlaying;
};