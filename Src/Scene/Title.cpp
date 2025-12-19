#include "Title.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"

// コンストラクタ
Title::Title(void) : SceneBase()
{
	for (int ii = 0; ii < MENU_NUM; ii++)
	{
		stringLeftUpPos[ii]		= { 0, 0 };
		stringRightDownPos[ii]  = { 0, 0 };
		menuChoice[ii]			= -1;
		menuNotChoice[ii]		= -1;
		menuClick[ii]			= -1;
		clickFlg[ii]			= false;

		list[ii].textLeftUpPos_	   = stringLeftUpPos[ii];
		list[ii].textRightDownPos_ = stringRightDownPos[ii];
		list[ii].choicePic		   = menuChoice[ii];
		list[ii].notChoicePic	   = menuNotChoice[ii];
		list[ii].clickPic		   = menuClick[ii];
		list[ii].textFlg_		   = false;
	}

	haikei_			    = -1;
	gamestart_choice	= -1;
	gamestart_notchoice = -1;
	gamestart_click     = -1;
	keyhelp_choice		= -1;
	keyhelp_notchoice	= -1;
	keyhelp_click	    = -1;

	isSEPlaying = false;

}

// デストラクタ
Title::~Title(void)
{
}

// 初期化
void Title::Init(void)
{
	// 画像の読み込み
	haikei_				= LoadGraph((Application::PATH_IMAGE + "Title.png").c_str());

	gamestart_choice	= LoadGraph((Application::PATH_IMAGE + "gamestart_choice.png").c_str());
	gamestart_notchoice = LoadGraph((Application::PATH_IMAGE + "gamestart_notchoice.png").c_str());
	gamestart_click     = LoadGraph((Application::PATH_IMAGE + "gamestart_choice_click.png").c_str());
	
	keyhelp_choice		= LoadGraph((Application::PATH_IMAGE + "keyhelp_choice.png").c_str());
	keyhelp_notchoice	= LoadGraph((Application::PATH_IMAGE + "keyhelp_notchoice.png").c_str());
	keyhelp_click		= LoadGraph((Application::PATH_IMAGE + "keyhelp_choice_click.png").c_str());

	gameend_choice      = LoadGraph((Application::PATH_IMAGE + "gameend_choice.png").c_str());
	gameend_notchoice   = LoadGraph((Application::PATH_IMAGE + "gameend_notchoice.png").c_str());
	gameend_click       = LoadGraph((Application::PATH_IMAGE + "gameend_choice_click.png").c_str());

	//-----------------------------
	// 文字情報配列の初期化
	//-----------------------------
	// 左上座標
	stringLeftUpPos[0] = { (int)GAMESTART_TEXT_X, (int)GAMESTART_TEXT_Y };
	stringLeftUpPos[1] = { (int)KEYHELP_TEXT_X,   (int)KEYHELP_TEXT_Y   };
	stringLeftUpPos[2] = { (int)GAMEEND_TEXT_X,   (int)GAMEEND_TEXT_Y   };

	// 右下座標
	stringRightDownPos[0] = { (int)GAMESTART_TEXT_X_END, (int)GAMESTART_TEXT_Y_END };
	stringRightDownPos[1] = { (int)KEYHELP_TEXT_X_END,   (int)KEYHELP_TEXT_Y_END   };
	stringRightDownPos[2] = { (int)GAMEEND_TEXT_X_END,   (int)GAMEEND_TEXT_Y_END   };

	// 選択状態の画像
	menuChoice[0]	 = { gamestart_choice };
	menuChoice[1]	 = { keyhelp_choice	  };
	menuChoice[2]	 = { gameend_choice	  };

	// 非選択状態の画像
	menuNotChoice[0] = { gamestart_notchoice };
	menuNotChoice[1] = { keyhelp_notchoice	 };
	menuNotChoice[2] = { gameend_notchoice };

	// 決定状態の画像
	menuClick[0] = { gamestart_click };
	menuClick[1] = { keyhelp_click   };
	menuClick[2] = { gameend_click   };

	// 決定フラグ
	clickFlg[0] = false;
	clickFlg[1] = false;
	clickFlg[2] = false;

	//-----------------------------
	// 構造体変数の初期化
	//-----------------------------
	for (int ii = 0; ii < MENU_NUM; ii++)
	{
		list[ii].textLeftUpPos_	   = stringLeftUpPos[ii];		// 左上
		list[ii].textRightDownPos_ = stringRightDownPos[ii];	// 右下
		list[ii].choicePic		   = menuChoice[ii];			// 選択状態の画像
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
void Title::Update(void)
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
			if (nextScene == SceneManager::SCENE_ID::GAME)
			{
				sceneIns.ChangeScene(SceneManager::SCENE_ID::GAME);
			}
			else if (nextScene == SceneManager::SCENE_ID::KEYHELP)
			{
				sceneIns.ChangeScene(SceneManager::SCENE_ID::KEYHELP);
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

				if (list[0].textFlg_)
				{
					nextScene = SceneManager::SCENE_ID::GAME;
				}
				if (list[1].textFlg_)
				{
					nextScene = SceneManager::SCENE_ID::KEYHELP;
				}
			}
		}
	}
}

// 描画
void Title::Draw(void)
{
	// 背景画像の描画
	DrawRotaGraph((int)Application::HALF_SCREEN_SIZE_X, (int)Application::HALF_SCREEN_SIZE_Y,
				  1.0f, 0.0f, haikei_, true, false);
	
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
void Title::Release(void)
{
	// シングルトンの呼び出し
	SoundManager* seMana_ = SoundManager::GetInstance();

	// BGMの停止
	seMana_->StopBGM("TitleBGM");

	// 解放処理
	seMana_->ReleaseSound("TitleBGM");

	// 画像の解放
	DeleteGraph(haikei_);

	DeleteGraph(gamestart_choice);
	DeleteGraph(gamestart_notchoice);
	DeleteGraph(gamestart_click);

	DeleteGraph(keyhelp_choice);
	DeleteGraph(keyhelp_notchoice);
	DeleteGraph(keyhelp_click);
	
	DeleteGraph(gameend_choice);
	DeleteGraph(gameend_notchoice);
	DeleteGraph(gameend_click);
}

