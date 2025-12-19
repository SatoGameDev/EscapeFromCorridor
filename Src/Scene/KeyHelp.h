#pragma once
#include <DxLib.h>
#include <string>
#include "Base/SceneBase.h"
#include "../Application.h"
#include "../Common/Vector2.h"

class KeyHelp : public SceneBase
{
public:
	//--------------
	// テキスト関連
	//--------------
	// タイトルに戻る
	static constexpr float TITLE_TEXT_X = 1100.0f;						// 左上 X座標
	static constexpr float TITLE_TEXT_Y = 750.0f;						//		Y座標
	static constexpr float TITLE_TEXT_X_END = TITLE_TEXT_X + 350.0f;	// 右下 X座標
	static constexpr float TITLE_TEXT_Y_END = TITLE_TEXT_Y + 60.0f;		//		Y座標

	// メニュー項目の数
	static constexpr int MENU_NUM = 1;

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

	KeyHelp(void);					// コンストラクタ
	~KeyHelp(void) override;		// デストラクタ

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
	int     menuClick[MENU_NUM];			// メニュー決定状態の画像
	bool    clickFlg[MENU_NUM];				// メニュー決定フラグ

	// 画像のハンドル
	int haikei_;
	int titleback_choice;
	int titleback_notchoice;
	int titleback_click;

	// SEが再生中かどうか示すフラグ
	bool isSEPlaying;

};