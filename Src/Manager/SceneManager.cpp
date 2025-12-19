#include <chrono>
#include <DxLib.h>
#include "SceneManager.h"
#include "../Common/Fader.h"
#include "Camera.h"
#include "../Object/Stage.h"
#include "../Object/Enemy.h"
#include "../Scene/Title.h"
#include "../Scene/KeyHelp.h"
#include "../Scene/Game.h"
#include "../Scene/GameOver.h"
#include "../Scene/GameClear.h"


SceneManager* SceneManager::instance_ = nullptr;


// 明示的にインスタンスを生成する
void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

// 静的インスタンスの取得
SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

// 初期化
void SceneManager::Init(void)
{

	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	fader_ = new Fader();
	fader_->Init();

	//カメラ初期化
	camera_ = new Camera();
	camera_->Init();

	isSceneChanging_ = false;

	// GameSceneかどうか
	gameFlg_ = false;

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	// 初期シーンの設定
	DoChangeScene(SCENE_ID::TITLE);

	// ステージモデルの読み込み
	stageModel   = MV1LoadModel("Data/Model/Stage/stage.mv1");
	floorModel   = MV1LoadModel("Data/Model/Stage/floor/floor.mv1");
	ceilingModel = MV1LoadModel("Data/Model/Stage/ceiling/ceiling.mv1");

	Init3D();

}

// 初期化(3D)
void SceneManager::Init3D(void)
{
	// 背景色設定
	SetBackgroundColor(105, 20, 100);

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	//ライトを有効にする
	SetUseLighting(true);
	SetLightPosition(VGet(0.0f, 500.0f, 0.0f));

	// 正面から斜めに下に向かったライト
	ChangeLightTypeDir({ 2.00f,2.00f,2.00f });

	// フォグ設定(ステージ導入時)
	// フォグの使用するかどうか
	//SetFogEnable(true);
	// フォグの色の変更(R,G,B)
	SetFogColor(5, 5, 5);
	// フォグの開始距離と終了する距離の設定
	SetFogStartEnd(1000.0f, 1500.0f);
}

// 更新
void SceneManager::Update(void)
{

	if (scene_ == nullptr)
	{
		return;
	}

	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	fader_->Update();
	if (isSceneChanging_)
	{
		Fade();
	}
	else
	{
		scene_->Update();
	}

	if (gameFlg_)
	{
		SetMouseDispFlag(false);
		camera_->GameUpdate();
	}
	else
	{
		SetMouseDispFlag(true);
		camera_->Update();
	}
}

// 描画
void SceneManager::Draw(void)
{

	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();

	//カメラ設定
	camera_->SetBeforeDraw();

	// 描画
	scene_->Draw();

	//カメラデバック等
	camera_->Draw();

	// 暗転・明転
	fader_->Draw();

}

// 解放
void SceneManager::Release(void)
{
	MV1DeleteModel(stageModel);
	MV1DeleteModel(floorModel);
	MV1DeleteModel(ceilingModel);

	scene_->Release();
	delete scene_;

	delete fader_;

}

// 状態遷移
void SceneManager::ChangeScene(SCENE_ID nextId)
{

	// フェード処理が終わってからシーンを変える場合もあるため、
	// 遷移先シーンをメンバ変数に保持
	waitSceneId_ = nextId;

	// フェードアウト(暗転)を開始する
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;

}

// シーンIDの取得
SceneManager::SCENE_ID SceneManager::GetSceneID(void)
{
	return sceneId_;
}

// デルタタイムの取得
float SceneManager::GetDeltaTime(void) const
{
	//return 1.0f / 60.0f;
	return deltaTime_;
}

// カメラの取得
Camera* SceneManager::GetCamera(void) const
{
	return camera_;
}

// コンストラクタ
SceneManager::SceneManager(void)
{
	// シーンIDの初期化
	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	scene_  = nullptr;
	fader_  = nullptr;
	camera_ = nullptr;

	isSceneChanging_ = false;
	gameFlg_ = false;

	// デルタタイム
	deltaTime_ = 1.0f / 60.0f;
}

// デストラクタ
SceneManager::~SceneManager(void)
{
	delete instance_;
}

// デルタタイムをリセットする
void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

// シーン遷移
void SceneManager::DoChangeScene(SCENE_ID sceneId)
{

	// シーンを変更する
	sceneId_ = sceneId;

	// 現在のシーンを解放
	if (scene_ != nullptr)
	{
		scene_->Release();
		delete scene_;
	}

	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		gameFlg_ = false;
		scene_ = new Title();
		break;
	case SCENE_ID::KEYHELP:
		gameFlg_ = false;
		scene_ = new KeyHelp();
		break;
	case SCENE_ID::GAME:
		camera_->ReSet();
		gameFlg_ = true;
		scene_ = new Game();
		break;
	case SCENE_ID::GAMEOVER:
		gameFlg_ = false;
		scene_ = new GameOver();
		break;
	case SCENE_ID::GAMECLEAR:
		gameFlg_ = false;
		scene_ = new GameClear();
		break;
	}

	scene_->Init();

	// デルタタイムをリセットする
	ResetDeltaTime();

	waitSceneId_ = SCENE_ID::NONE;

}

// フェード
void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// 明転中
		if (fader_->IsEnd())
		{
			// 明転が終了したら、フェード処理終了
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// 暗転中
		if (fader_->IsEnd())
		{
			// 完全に暗転してからシーン遷移
			DoChangeScene(waitSceneId_);
			// 暗転から明転へ
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}
}


