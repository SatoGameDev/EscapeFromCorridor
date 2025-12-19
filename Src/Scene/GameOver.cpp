#include "GameOver.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"


namespace
{
	constexpr int RESTART_IDX = static_cast<int>(GameOver::UI_TYPE::RESTART);
	constexpr int BACK_TITLE = static_cast<int>(GameOver::UI_TYPE::BACK_TITLE);
	constexpr int UI_TYPE_NUM = static_cast<int>(GameOver::UI_TYPE::UI_TYPE_NUM);
}

// コンストラクタ
GameOver::GameOver(void) : SceneBase()
{
	haikei_				= -1;
	restart_choice		= -1;
	restart_notchoice	= -1;
	restart_click	    = -1;
	titleback_choice	= -1;
	titleback_notchoice = -1;
	titleback_click	    = -1;

	isSEPlaying = false;

}

// デストラクタ
GameOver::~GameOver(void)
{
}

// 初期化
void GameOver::Init(void)
{
	// 画像の読み込み
	haikei_			    = LoadGraph((Application::PATH_IMAGE + "gameover.png").c_str());
	restart_choice		= LoadGraph((Application::PATH_IMAGE + "restart_choice.png").c_str());
	restart_notchoice	= LoadGraph((Application::PATH_IMAGE + "restart_notchoice.png").c_str());
	restart_click		= LoadGraph((Application::PATH_IMAGE + "restart_choice_click.png").c_str());
	titleback_choice	= LoadGraph((Application::PATH_IMAGE + "titleback_choice_over.png").c_str());
	titleback_notchoice = LoadGraph((Application::PATH_IMAGE + "titleback_notchoice_over.png").c_str());
	titleback_click		= LoadGraph((Application::PATH_IMAGE + "titleback_choice_over_click.png").c_str());

	//-----------------------------
	// 文字情報配列の初期化
	//-----------------------------
	
	// 動的に生成
	for (int i = 0; i < UI_TYPE_NUM; ++i)
	{
		SceneBase::text uiData;
		uiDatalist.push_back(uiData);
	}

	// 左上座標
	uiDatalist[RESTART_IDX].textLeftUpPos_ = { (int)RESTART_TEXT_X, (int)RESTART_TEXT_Y };
	uiDatalist[RESTART_IDX].textRightDownPos_ = { (int)RESTART_TEXT_X_END, (int)RESTART_TEXT_Y_END };
	uiDatalist[RESTART_IDX].choicePic =  restart_choice ;
	uiDatalist[RESTART_IDX].notChoicePic = restart_notchoice ;
	uiDatalist[RESTART_IDX].clickPic = restart_click;
	uiDatalist[RESTART_IDX].clickFlg_ = false;

	// 右下座標
	uiDatalist[BACK_TITLE].textRightDownPos_ = { (int)TITLE_TEXT_X_END,   (int)TITLE_TEXT_Y_END };
	uiDatalist[BACK_TITLE].textLeftUpPos_ = { (int)TITLE_TEXT_X,   (int)TITLE_TEXT_Y };
	uiDatalist[BACK_TITLE].choicePic = titleback_choice;
	uiDatalist[BACK_TITLE].notChoicePic = titleback_notchoice;
	uiDatalist[BACK_TITLE].clickPic = titleback_click;
	uiDatalist[BACK_TITLE].clickFlg_ = false;

	// SEが再生中かどうか示すフラグ
	isSEPlaying = false;
}

// 更新
void GameOver::Update(void)
{
	// インスタンス取得
	InputManager& inputIns = InputManager::GetInstance();
	SceneManager& sceneIns = SceneManager::GetInstance();

	// シングルトンの呼び出し
	SoundManager* seMana_ = SoundManager::GetInstance();
	// 次に遷移するシーン
	static SceneManager::SCENE_ID nextScene = SceneManager::SCENE_ID::NONE;

	// SEが再生中の場合、再生終了を待つ
	if (isSEPlaying)
	{
		// SEの再生がすんだのかチェック
		if (!seMana_->IsPlayingSE("click"))
		{
			// 保存しておいたシーンに遷移
			if (nextScene == SceneManager::SCENE_ID::GAME)
			{
				sceneIns.ChangeScene(SceneManager::SCENE_ID::GAME);
			}
			else if ((nextScene == SceneManager::SCENE_ID::TITLE))
			{
				sceneIns.ChangeScene(SceneManager::SCENE_ID::TITLE);
			}
		}
		// SE再生中ならUpdateを終了
		return;
	}

	for (int ii = 0; ii < UI_TYPE_NUM; ii++)
	{
		// マウスカーソルが文字列の範囲内に入っているか
		if (uiDatalist[ii].textFlg_ = IsCheckCursor(uiDatalist[ii].textLeftUpPos_.x, uiDatalist[ii].textRightDownPos_.x,
			uiDatalist[ii].textLeftUpPos_.y, uiDatalist[ii].textRightDownPos_.y))
		{
			// シーン遷移
			if (inputIns.IsTrgMouseLeft())
			{
				// SEの音量の設定
				seMana_->SetVolumeSE("click", 200);
				// SEの再生
				seMana_->PlaySE("click");
				isSEPlaying = true;
				uiDatalist[ii].clickFlg_ = uiDatalist[ii].textFlg_;

				if (uiDatalist[0].textFlg_)
				{
					nextScene = SceneManager::SCENE_ID::GAME;
				}
				if (uiDatalist[1].textFlg_)
				{
					nextScene = SceneManager::SCENE_ID::TITLE;
				}
			}
		}
	}
}

// 描画
void GameOver::Draw(void)
{
	// 背景画像の描画
	DrawGraph(0, 0, haikei_, true);

	// メニュー項目テキストの描画
	for (int ii = 0; ii < UI_TYPE_NUM; ii++)
	{
		if (uiDatalist[ii].clickFlg_)
		{
			DrawGraph(uiDatalist[ii].textLeftUpPos_.x, uiDatalist[ii].textLeftUpPos_.y,
				uiDatalist[ii].clickPic, true);
		}
		else if (uiDatalist[ii].textFlg_)
		{
			DrawGraph(uiDatalist[ii].textLeftUpPos_.x, uiDatalist[ii].textLeftUpPos_.y,
				uiDatalist[ii].choicePic, true);
		}
		else
		{
			DrawGraph(uiDatalist[ii].textLeftUpPos_.x, uiDatalist[ii].textLeftUpPos_.y,
				uiDatalist[ii].notChoicePic, true);
		}
	}
}

// 解放
void GameOver::Release(void)
{
	// SEが再生中かどうか示すフラグ
	isSEPlaying = false;

	// 画像の解放
	DeleteGraph(haikei_);
	DeleteGraph(restart_choice);
	DeleteGraph(restart_notchoice);
	DeleteGraph(restart_click);
	DeleteGraph(titleback_choice);
	DeleteGraph(titleback_notchoice);
	DeleteGraph(titleback_click);

	// 動的配列削除
	uiDatalist.clear();
}
