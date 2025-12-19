#pragma once
#include <DxLib.h>
#include <string>
#include "Base/SceneBase.h"
#include "../Application.h"
#include "../Common/Vector2.h"

class GameOver : public SceneBase
{
public:
	//--------------
	// テキスト関連
	//--------------
	// リスタート
	static constexpr float RESTART_TEXT_X = Application::HALF_SCREEN_SIZE_X - 113.0f;		// 左上 X座標
	static constexpr float RESTART_TEXT_Y = Application::HALF_SCREEN_SIZE_Y + 100.0f;		//		Y座標
	static constexpr float RESTART_TEXT_X_END = RESTART_TEXT_X + 226.0f;					// 右下 X座標
	static constexpr float RESTART_TEXT_Y_END = RESTART_TEXT_Y + 37.0f;						//		Y座標

	// タイトルに戻る
	static constexpr float TITLE_TEXT_X = Application::HALF_SCREEN_SIZE_X - 166.5f;			// 左上 X座標
	static constexpr float TITLE_TEXT_Y = RESTART_TEXT_Y + 80.0f;							//		Y座標
	static constexpr float TITLE_TEXT_X_END = TITLE_TEXT_X + 333.0f;						// 右下 X座標
	static constexpr float TITLE_TEXT_Y_END = TITLE_TEXT_Y + 44.0f;							//		Y座標
	

	// UIの数
	enum class UI_TYPE
	{
		RESTART,
		BACK_TITLE,

		UI_TYPE_NUM,	// メニュー項目の数
	};
	

	GameOver(void);					// コンストラクタ
	~GameOver(void) override;		// デストラクタ

	// 初期処理
	void Init(void) override;		// 初期化
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void Release(void) override;	// 解放


private:

	// 画像のハンドル
	int haikei_;		// 背景
	int restart_choice;
	int restart_notchoice;
	int restart_click;
	int titleback_choice;
	int titleback_notchoice;
	int titleback_click;

	// SEが再生中かどうか示すフラグ
	bool isSEPlaying;

};