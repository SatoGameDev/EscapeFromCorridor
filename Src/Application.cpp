#include <DxLib.h>
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "Manager/SoundManager.h"
#include "Application.h"

// インスタンスの生成
Application* Application::instance_ = nullptr;

// データパス関連
const std::string Application::PATH_MODEL  = "Data/Model/";
const std::string Application::PATH_IMAGE  = "Data/Image/";
const std::string Application::PATH_SOUND  = "Data/Sound/";
const std::string Application::PATH_EFFECT = "Data/Effekseer/";

// 明示的にインスタンスを生成する
void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

// 静的インスタンスの取得
Application& Application::GetInstance(void)
{
	return *instance_;
}

// 初期化
void Application::Init(void)
{
	// アプリケーションの初期設定
	SetWindowText("EscapingFromCorridor");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);

	// ウィンドウモードの設定
	// true : ウィンドウ、false : フルスクリーン
	ChangeWindowMode(true);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;

	SetUsePixelLighting(true);

	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}


	SetDrawScreen(DX_SCREEN_BACK);

	// キー制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// SE管理の開放処理
	SoundManager::CreateInstance();
	SoundManager::GetInstance()->LoadSE("click", (Application::PATH_SOUND + "SE/Mouse.mp3").c_str(), false);

	// シーン管理初期化
	SceneManager::CreateInstance();

	// フレームレート関連
	currentTime = 0;
	lastFrameTime = 0;
	frameCnt = 0;
	updateFrameRateTime = 0;
	frameRate = 0.0f;
}

// ゲームループの開始
void Application::Run(void)
{
	auto& inputManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		Sleep(1);
		currentTime = GetNowCount();

		if (currentTime - lastFrameTime >= FRAME_RATE)
		{
			lastFrameTime = currentTime;
			frameCnt++;

			// 画面を初期化
			ClearDrawScreen();

			inputManager.Update();
			sceneManager.Update();
			sceneManager.Draw();

			CalcFrameRate();
			//DrawFrameRate();

			if (sceneManager.GetSceneID() == SceneManager::SCENE_ID::TITLE)
			{
				Vector2 mousePos = inputManager.GetMousePos();

				if (mousePos.x >= 1000 && mousePos.x <= 1350 &&
					mousePos.y >= 690 && mousePos.y <= 750)
				{
					if (inputManager.IsTrgMouseLeft())
					{
						break;
					}
				}
			}

			ScreenFlip();
		}
	}
}

// 解放
void Application::Release(void)
{
	// SE解放
	SoundManager::GetInstance()->StopSE("click");

	// 解放処理
	SoundManager::GetInstance()->ReleaseSound("click");

	// SE管理の開放処理
	SoundManager* soundMng = SoundManager::GetInstance();
	soundMng->DeleteInstance();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}
}

// 初期化成功／失敗の判定
bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

// 解放成功／失敗の判定
bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

// デフォルトコンストラクタをprivateにして、
// 外部から生成できない様にする
Application::Application(void)
{
	// フラグの初期化
	isInitFail_ = false;
	isReleaseFail_ = false;

	// フレームレート関連
	currentTime = 0;
	lastFrameTime = 0;
	frameCnt = 0;
	updateFrameRateTime = 0;
	frameRate = 0.0f;
}

// デストラクタも同様
Application::~Application(void)
{
	// インスタンスの解放
	delete instance_;
}

// フレームレートの計算
void Application::CalcFrameRate()
{
	int nDifTime = currentTime - updateFrameRateTime;

	if (nDifTime > 1000)
	{
		float fFrameCnt = (float)(frameCnt * 1000);
		frameRate = fFrameCnt / nDifTime;
		frameCnt = 0;
		updateFrameRateTime = currentTime;
	}
}

// フレームレートの描画
void Application::DrawFrameRate()
{
	//DrawFormatString(SCREEN_SIZE_X - 90, 0, GetColor(255, 30, 30), "FPS[%.2f]", frameRate);
}
