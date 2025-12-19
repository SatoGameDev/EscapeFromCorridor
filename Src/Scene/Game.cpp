#include <DxLib.h>
#include <cmath>
#include "../Common/Library.h"
#include "../Common/Vector2.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/Camera.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Stage.h"
#include "../Object/Aitem.h"
#include "../Object/Magatama.h"
#include "../Object/Grid.h"
#include "../Object/Light.h"
#include "Base/SceneBase.h"
#include "Game.h"



// コンストラクタ
Game::Game(void)
	:reticle_(-1), modelOriginalHandle_(-1),
	 mNum_(-1), shadowMapHandle_(-1),
	 camera_(nullptr), grid_(nullptr), stage_(nullptr),
	 player_(nullptr), enemy_(nullptr), mtama_(nullptr),
	 aitem_(nullptr), light_(nullptr)
{
}

// デストラクタ
Game::~Game(void)
{
}

// 初期化
void Game::Init(void)
{
	// 勾玉の取得数の初期化
	mNum_ = 0;

	// 読み込み
	reticle_ = LoadGraph((Application::PATH_IMAGE +  "circle_6.png").c_str());		
	keyhelp_ = LoadGraph((Application::PATH_IMAGE +  "keyhelp_ingame.png").c_str());
	warp	 = LoadGraph((Application::PATH_IMAGE + "warping.png").c_str());
	letsget  = LoadGraph((Application::PATH_IMAGE + "letsget.png").c_str());
	gotorii  = LoadGraph((Application::PATH_IMAGE + "gotorii.png").c_str());
	maga[0]  = LoadGraph((Application::PATH_IMAGE + "maga0.png").c_str());
	maga[1]  = LoadGraph((Application::PATH_IMAGE + "maga1.png").c_str());
	maga[2]  = LoadGraph((Application::PATH_IMAGE + "maga2.png").c_str());
	maga[3]  = LoadGraph((Application::PATH_IMAGE + "maga3.png").c_str());
	magatama = LoadGraph((Application::PATH_IMAGE + "magatama.png").c_str());

	modelOriginalHandle_ = MV1LoadModel((Application::PATH_MODEL + "magatama.mv1").c_str());	// 勾玉の複製元モデル

	// 初期化
	camera_ = new Camera();		// カメラ
	camera_->Init();

	grid_ = new Grid();	// グリッド初期化
	grid_->Init();

	stage_ = new Stage();	// ステージ初期化
	stage_->Init(stagemodel);

	player_ = new Player();	// プレイヤー初期化
	player_->Init();

	enemy_ = new Enemy();	// エネミー初期化
	enemy_->Init();
	enemy_->SetPlayer(player_);

	mtama_ = new Magatama();	// 勾玉初期化
	mtama_->Init(modelOriginalHandle_);

	aitem_ = new Aitem();	// アイテムの初期化
	aitem_->Init();
	aitem_->SetPlayer(player_);
	aitem_->SetEnemy(enemy_);
	aitem_->SetStage(stage_);

	light_ = new Light();	// ライト
	light_->Init();
	light_->SetPlayer(player_);

	// カメラの追従対象を設定
	SceneManager& sceneManager = SceneManager::GetInstance();
	Camera* camera = sceneManager.GetCamera();
	camera->SetPlayer(player_);

	stagemodel =  sceneManager.GetWallModelId();
	floormodel = sceneManager.GetFloorModelId();
	ceilingmodel = sceneManager.GetCeilingModelId();

	// 当たり判定(コライダ)作成
	for (int ii = 0; ii < Magatama::MAGATAMA_MAX_NUM; ii++)
	{
		MV1SetupCollInfo(mtama_->GetModelId(ii), -1, 1, 1, 1);
	}

	MV1SetupCollInfo(enemy_->GetInfo().modelInfo_.modelId_, -1, 1, 1, 1);

	// シャドウマップハンドルの作成
	shadowMapHandle_ = MakeShadowMap(4096, 4096);

	// シャドウマップが想定するライトの方向もセット
	SetShadowMapLightDirection(shadowMapHandle_, VGet(0.00f, 0.00f, 1.00f));

	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(shadowMapHandle_, VGet(0.0f, 0.0f, 0.0f), VGet(12000.0f, 1000.0f, 12000.0f));


	// シングルトンの呼び出し
	SoundManager* seMana_ = SoundManager::GetInstance();

	// 勾玉をとる音
	seMana_->LoadSE("Take", (Application::PATH_SOUND + "SE/se_itemget.wav").c_str(), false);

}

// 更新
void Game::Update(void)
{
	// カメラ更新
	camera_->Update();

	// グリッド更新
	grid_->Update();

	// ステージ更新
	stage_->Update();

	// 勾玉更新
	mtama_->Update();

	aitem_->Update();

	light_->Update();

	// プレイヤー更新
	player_->Update(stagemodel);

	// エネミー更新
	enemy_->Update(stagemodel);
	
	// 入力制御取得
	InputManager& ins = InputManager::GetInstance();

	// シーン遷移
	VECTOR pPos_ = player_->GetInfo().pos_;
	if (mNum_ >= Magatama::MAGATAMA_MAX_NUM && (pPos_.x > 5740.0f && pPos_.z >5740.0f))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);
	}


	// プレイヤーの生存フラグがfalseだったら
	if (!player_->GetInfo().aliveFlg_)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}



	// 当たり判定更新
	Collision(stagemodel);
}

// 描画
void Game::Draw(void)
{
	// ステージ描画
	stage_->Draw(stagemodel, floormodel, ceilingmodel, mNum_);
	
	//--------------------------------------------------
	// シャドウマップへの描画の準備
	//--------------------------------------------------
	ShadowMap_DrawSetup(shadowMapHandle_);

	player_->Draw();// プレイヤー描画
	
	mtama_->Draw();	// 勾玉描画

	// シャドウマップマップへの描画を終了
	ShadowMap_DrawEnd();
	//--------------------------------------------------

	//--------------------------------------------------
	// 描画に使用するシャドウマップを設定
	//--------------------------------------------------
	SetUseShadowMap(0, shadowMapHandle_);

	player_->Draw();	// プレイヤー描画
	//enemy_->Draw();	// エネミー描画
	mtama_->Draw();	// 勾玉描画

	// 描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(0, -1);
	//--------------------------------------------------

	aitem_->Draw();

	enemy_->Draw();	// エネミー描画

	light_->Draw();

	// カメラ設定
	camera_->SetBeforeDraw();

	// レティクルの描画
	DrawRotaGraph((int)Application::HALF_SCREEN_SIZE_X, (int)Application::HALF_SCREEN_SIZE_Y,
			   	  1.0f, 0.0f, reticle_, true, false);
	DrawGraph(0, 0, keyhelp_, true);

	DrawGraph(300, 5, magatama, true);
	DrawGraph(320, 5, maga[mNum_], true);

	if (mNum_ < 3)
	{
		DrawGraph(5, 5, letsget, true);
	}
	else
	{
		DrawGraph(5, 5, gotorii, true);

		VECTOR pPos_ = player_->GetInfo().pos_;
		rot -= 5.0f * Application::DEG_TO_RAD;
		if (rot < -360.0f * Application::DEG_TO_RAD)
		{
			rot = 0.0f;
		}

		if (pPos_.x > 5740.0f && pPos_.z > 5740.0f)
		{
			DrawRotaGraph2(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2,
				925, 925,
				1.0f,
				rot,
				warp,
				true, false
			);
		}
	}
}

// 解放
void Game::Release(void)
{
	// シングルトンの呼び出し
	SoundManager* seMana_ = SoundManager::GetInstance();

	// SEの停止
	seMana_->StopSE("Take");

	// 解放処理
	seMana_->ReleaseSound("Take");

	// シャドウマップの削除
	DeleteShadowMap(shadowMapHandle_);


	// カメラ解放
	camera_->Release();
	delete camera_;

	// グリッド解放
	grid_->Release();
	delete grid_;

	// ステージ解放
	stage_->Release();
	delete stage_;

	// プレイヤー解放
	player_->Release();
	delete player_;

	// エネミー解放
	enemy_->Release();
	delete enemy_;

	// 勾玉解放
	mtama_->Release();
	delete mtama_;

	aitem_->Release();
	delete aitem_;

	light_->Release();
	delete light_;	
}

// デバッグ描画
void Game::DrawDebug(void)
{
	// デバッグ
	player_->DrawDebug();		// プレイヤー
	enemy_-> DrawDebug();		// 敵
	camera_->Draw();			// カメラ
	mtama_-> DrawDebug();		// 勾玉

	// 勾玉の取得数の描画
	DrawFormatString(Application::SCREEN_SIZE_X - 160, 32, 0xffffff, "mNum_ : %d / %d", mNum_, Magatama::MAGATAMA_MAX_NUM);

}

// 当たり判定
void Game::Collision(int stagemodel)
{
	CollPlayerToEnemy();	 // プレイヤー	 と 敵の当たり判定
	CollLockCapToMagatama(); // 視線カプセル と 勾玉の当たり判定
	CollAitemToWall(stagemodel);		 // アイテム	 と 壁の当たり判定
	CollAitemToEnemy();	 	 // アイテム	 と 敵の当たり判定
}

// プレイヤーと敵の当たり判定
void Game::CollPlayerToEnemy(void)
{
	auto plCap_ = player_->GetInfo().cap;
	auto enCap_ = enemy_->GetInfo().cap;

	if (HitCheck_Capsule_Capsule(
		plCap_.start, plCap_.end, plCap_.radius,
		enCap_.start, enCap_.end, enCap_.radius))
	{
		// プレイヤーと敵の速度をゼロにして停止させる
		player_->SetPlayerSpeed(Application::DEFAULT_VECTOR);
		enemy_->SetEnemySpeed(Application::DEFAULT_VECTOR);

		// ダメージをセットする
		player_->SetDamage(100);
	}
}

// 視線カプセルと勾玉の当たり判定
void Game::CollLockCapToMagatama(void)
{
	// シングルトンの呼び出し
	SoundManager* seMana_ = SoundManager::GetInstance();
	seMana_->SetVolumeSE("Take", 255);

	// 視線ベクトルと勾玉との当たり判定
	for (int ii = 0; ii < Magatama::MAGATAMA_MAX_NUM; ii++)
	{
		// 勾玉とカプセル
		auto info = MV1CollCheck_Capsule(
			mtama_->GetModelId(ii), -1,
			player_->GetInfo().cPos_,
			player_->GetInfo().bPos_,
			20.0f
		);

		// 勾玉とカプセルが当たっていたら
		if (info.HitNum >= 1)
		{
			// 入力制御取得
			InputManager& ins = InputManager::GetInstance();

			// ビルボードを表示
			mtama_->SetDrawBillFlg(ii, true);

			if (ins.IsTrgDown(KEY_INPUT_F))
			{
				if (mtama_->GetDrawFlg(ii))
				{
					mtama_->SetDrawFlg(ii, false);
					mNum_ += 1;
					seMana_->PlaySE("Take");
				}
			}
		}
		else
		{
			// ビルボードを非表示
			mtama_->SetDrawBillFlg(ii, false);
		}

		// 当たり判定結果ポリゴン配列の後始末をする
		MV1CollResultPolyDimTerminate(info);
	}
}



// アイテムと壁の当たり判定
void Game::CollAitemToWall(int stagemodel)
{
	auto info = MV1CollCheck_Sphere(
		stagemodel,
		-1,
		aitem_->GetInfo().modelInfo_.pos_,
		25.0f
	);

	// 壁とアイテムが衝突したら
	if (info.HitNum >= 1)
	{
		// 壁にめり込んでるので、プレイヤーと同様に、壁に当たらない場所までずらす必要性あり
		

		// 反射ベクトルを求める
		aitem_->SetTransformVec(info.Dim->Normal);
	}

	// 当たり判定結果ポリゴン配列の後始末をする
	MV1CollResultPolyDimTerminate(info);
}

// アイテムと敵の当たり判定
void Game::CollAitemToEnemy(void)
{
	// 弾に当たったら敵を止める
	if (HitCheck_Sphere_Capsule(aitem_->GetInfo().modelInfo_.pos_, 20.0f,
								enemy_->GetInfo().cap.start,
								enemy_->GetInfo().cap.end,
								enemy_->GetInfo().cap.radius)
		)
	{
		// 敵の速度をゼロにして停止させる
		enemy_->SetStanFlg(true);
		aitem_->Blaser();
	}
}


