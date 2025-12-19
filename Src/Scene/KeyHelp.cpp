#include "KeyHelp.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"


// コンストラクタ
KeyHelp::KeyHelp(void)
{
	for (int ii = 0; ii < MENU_NUM; ii++)
	{
		stringLeftUpPos[ii]	   = { 0, 0 };
		stringRightDownPos[ii] = { 0, 0 };
		menuChoice[ii]		   = -1;
		menuNotChoice[ii]	   = -1;
		menuClick[ii]		   = -1;
		clickFlg[ii]		   = false;

		list[ii].textLeftUpPos_	   = stringLeftUpPos[ii];
		list[ii].textRightDownPos_ = stringRightDownPos[ii];
		list[ii].choicePic		   = menuChoice[ii];
		list[ii].notChoicePic	   = menuNotChoice[ii];
		list[ii].clickPic		   = menuClick[ii];
		list[ii].textFlg_		   = false;
	}

	haikei_				= -1;
	titleback_choice	= -1;
	titleback_notchoice = -1;
	titleback_click	    = -1;

	isSEPlaying = false;
}

// デストラクタ
KeyHelp::~KeyHelp(void)
{
}

// 初期化
void KeyHelp::Init(void)
{
	// 画像の読み込み
	haikei_			    = LoadGraph((Application::PATH_IMAGE + "keyhelp.png").c_str());
	titleback_choice    = LoadGraph((Application::PATH_IMAGE + "titleback_choice.png").c_str());
	titleback_notchoice = LoadGraph((Application::PATH_IMAGE + "titleback_notchoice.png").c_str());
	titleback_click	    = LoadGraph((Application::PATH_IMAGE + "titleback_choice_click.png").c_str());

	//-----------------------------
	// 文字情報配列の初期化
	//-----------------------------
	// 左上座標
	stringLeftUpPos[0]	  = { (int)TITLE_TEXT_X, (int)TITLE_TEXT_Y };

	// 右下座標
	stringRightDownPos[0] = { (int)TITLE_TEXT_X_END, (int)TITLE_TEXT_Y_END };

	// 選択状態の画像
	menuChoice[0]	 = { titleback_choice };

	// 非選択状態の画像
	menuNotChoice[0] = { titleback_notchoice };

	// 決定状態の画像
	menuClick[0] = { titleback_click };

	// 決定フラグ
	clickFlg[0] = false;

	//-----------------------------
	// 構造体変数の初期化
	//-----------------------------
	for (int ii = 0; ii < MENU_NUM; ii++)
	{
		list[ii].textLeftUpPos_    = stringLeftUpPos[ii];		// 左上
		list[ii].textRightDownPos_ = stringRightDownPos[ii];	// 右下
		list[ii].choicePic		   = menuChoice[ii];		    // 選択状態の画像
		list[ii].notChoicePic	   = menuNotChoice[ii];			// 非選択状態の画像
		list[ii].clickPic		   = menuClick[ii];				// 決定状態の画像
		list[ii].textFlg_		   = false;						// 選択フラグ
	}

	// シングルトンの呼び出し
	SoundManager* seMana_ = SoundManager::GetInstance();
	// BGMのロード
	seMana_->LoadBGM("TitleBGM", (Application::PATH_SOUND + "BGM/kowareta-waltz.mp3").c_str(),false);

	// SEが再生中かどうか示すフラグ
	isSEPlaying = false;
}

// 更新
void KeyHelp::Update(void)
{
	// インスタンス取得
	InputManager& inputIns = InputManager::GetInstance();
	SceneManager& sceneIns = SceneManager::GetInstance();

	// シングルトンの呼び出し
	SoundManager* seMana_ = SoundManager::GetInstance();
	// 次に遷移するシーン
	static SceneManager::SCENE_ID nextScene = SceneManager::SCENE_ID::NONE;

	// BGMの再生
	seMana_->SetVolumeSE("TitleBGM", 200);
	seMana_->PlayBGM("TitleBGM");

	// SEが再生中の場合、再生終了を待つ
	if (isSEPlaying)
	{
		// SEの再生がすんだのかチェック
		if (!seMana_->IsPlayingSE("click"))
		{
			seMana_->StopBGM("TitleBGM");
			// 保存しておいたシーンに遷移
			if (nextScene == SceneManager::SCENE_ID::TITLE)
			{
				sceneIns.ChangeScene(SceneManager::SCENE_ID::TITLE);
			}
		}
		// SE再生中ならUpdateを終了
		return;
	}

	for (int ii = 0; ii < MENU_NUM; ii++)
	{
		// マウスカーソルが文字列の範囲内に入っているか
		if (list[ii].textFlg_ = IsCheckCursor(list[ii].textLeftUpPos_.x, list[ii].textRightDownPos_.x,
											  list[ii].textLeftUpPos_.y, list[ii].textRightDownPos_.y))
		{
			// シーン遷移
			if (inputIns.IsTrgMouseLeft())
			{
				// SEの音量の設定
				seMana_->SetVolumeSE("click", 255);
				// SEの再生
				seMana_->PlaySE("click");
				isSEPlaying = true;
				clickFlg[ii] = list[ii].textFlg_;

				nextScene = SceneManager::SCENE_ID::TITLE;
			}
		}
	}
}

// 描画
void KeyHelp::Draw(void)
{
	// 背景画像の描画
	DrawGraph(0, 0, haikei_, true);

	// メニュー項目テキストの描画
	for (int ii = 0; ii < MENU_NUM; ii++)
	{
		if (clickFlg[ii])
		{
			DrawGraph(list[ii].textLeftUpPos_.x, list[ii].textLeftUpPos_.y,
					  list[ii].clickPic, true);
		}
		else if (list[ii].textFlg_)
		{
			DrawGraph(list[ii].textLeftUpPos_.x, list[ii].textLeftUpPos_.y,
					  list[ii].choicePic, true);
		}
		else
		{
			DrawGraph(list[ii].textLeftUpPos_.x, list[ii].textLeftUpPos_.y,
					  list[ii].notChoicePic, true);
		}
	}
}

// 解放
void KeyHelp::Release(void)
{
	// シングルトンの呼び出し
	SoundManager* seMana_ = SoundManager::GetInstance();

	// BGMの停止
	seMana_->StopBGM("TitleBGM");

	// 解放処理
	seMana_->ReleaseSound("TitleBGM");

	// 画像の解放
	DeleteGraph(haikei_);
	DeleteGraph(titleback_choice);
	DeleteGraph(titleback_notchoice);
	DeleteGraph(titleback_click);
}