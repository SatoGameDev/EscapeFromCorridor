#include <DxLib.h>
#include "../Common/Library.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"

#include "Player.h"
#include "Enemy.h"
#include "Stage.h"
#include "Aitem.h"

// コンストラクタ
Aitem::Aitem(void)
	:player_(nullptr), stage_(nullptr),
	 enemy_(nullptr)
{
}

// デストラクタ
Aitem::~Aitem(void)
{
}

// 初期化
void Aitem::Init(void)
{
	stage_ = new Stage();
	enemy_ = new Enemy();

	//-----------------------------
	// 構造体変数の初期化
	//-----------------------------
	aitemInfo_.modelInfo_.modelId_ = MV1LoadModel((Application::PATH_MODEL + "Aitem/ohuda/ohuda.mv1").c_str());	// モデルの読み込み
	aitemInfo_.modelInfo_.scale_   = Application::DEFAULT_SCALE;			// 大きさ設定
	aitemInfo_.modelInfo_.rot_	   = Application::DEFAULT_VECTOR;			// 角度設定
	aitemInfo_.modelInfo_.pos_	   = Application::DEFAULT_VECTOR;			// 初期位置設定
	aitemInfo_.oldPos_			   = Application::DEFAULT_VECTOR;			// 直前の座標
	aitemInfo_.localPos_		   = Application::DEFAULT_VECTOR;			// ローカル座標
	aitemInfo_.moveVec_			   = Application::DEFAULT_VECTOR;			// 移動ベクトル
	aitemInfo_.oldMoveVec_		   = Application::DEFAULT_VECTOR;			// 直前の移動ベクトル
	aitemInfo_.transformMoveVec_   = Application::DEFAULT_VECTOR;
	aitemInfo_.gravityPower_	   = 0.0f;									// 加算されていく重力の値
	aitemInfo_.time_			   = 0.0f;
	aitemInfo_.isShot_			   = false;									// 描画フラグ

	//-----------------------------
	// 大きさ・回転・座標の設定
	//-----------------------------
	MV1SetScale		 (aitemInfo_.modelInfo_.modelId_, aitemInfo_.modelInfo_.scale_);	// 大きさ設定
	MV1SetRotationXYZ(aitemInfo_.modelInfo_.modelId_, aitemInfo_.modelInfo_.rot_);		// 角度設定
	MV1SetPosition	 (aitemInfo_.modelInfo_.modelId_, aitemInfo_.modelInfo_.pos_);		// 初期位置設定
}

// 更新
void Aitem::Update()
{	
	//--------------------------------------------------------------------
	// カメラの座標から視点方向に向かってアイテムを投げる処理
	//--------------------------------------------------------------------
	VECTOR followPos = player_->GetInfo().pos_;
	aitemInfo_.localPos_ = VAdd(followPos, VGet(0.0f, 170.0f, 0.0f));

	aitemInfo_.moveVec_ = player_->GetInfo().bcVec_;// カメラ座標から視点先座標のベクトル

	
	// マウスの左クリックでアイテムの使用をする
	// マウスクリック時に生成する、一定時間後に削除する
	InputManager& ins = InputManager::GetInstance();

	if (!aitemInfo_.isShot_)
	{
		// プレイヤーの移動状態(歩いているのか、走っているのかなど)
		// に応じて、アイテムを投げるスピードを変更する
		blaserTime_ = BLASER_TIME;


		if (ins.IsClickMouseLeft())
		{
			aitemInfo_.isShot_ = true;
			aitemInfo_.gravityPower_ = 0.0f;

			aitemInfo_.modelInfo_.pos_ = aitemInfo_.localPos_;
			MV1SetPosition(aitemInfo_.modelInfo_.modelId_, aitemInfo_.modelInfo_.pos_);
			
			// 移動ベクトルを正規化し、スピードで大きさを設定する
			aitemInfo_.transformMoveVec_ = VNorm(aitemInfo_.moveVec_);
			aitemInfo_.transformMoveVec_ = VScale(aitemInfo_.transformMoveVec_, SPEED);

			SetRotation(player_->GetInfo().cAng_);
		}
	}
	else
	{
		if (aitemInfo_.modelInfo_.pos_.y < 10.0f || aitemInfo_.modelInfo_.pos_.y > 350.0f)
		{
			aitemInfo_.modelInfo_.pos_ = aitemInfo_.oldPos_;

			Blaser();
		}
		else
		{
			// 座標の更新
			aitemInfo_.modelInfo_.pos_ = VAdd(aitemInfo_.modelInfo_.pos_, aitemInfo_.transformMoveVec_);

			// 作成したベクトルのXZ成分はそのままに、Y成分だけを変化させる
			aitemInfo_.gravityPower_ += GRAVITY;
			aitemInfo_.modelInfo_.pos_.y -= aitemInfo_.gravityPower_;

			MV1SetPosition(aitemInfo_.modelInfo_.modelId_, aitemInfo_.modelInfo_.pos_);

			// 直前の移動ベクトルを設定する
			aitemInfo_.oldMoveVec_ = aitemInfo_.moveVec_;
			aitemInfo_.oldPos_ = aitemInfo_.modelInfo_.pos_;

			
		}
	}
}

// 描画
void Aitem::Draw(void)
{
	if (aitemInfo_.isShot_)
	{
		MV1DrawModel(aitemInfo_.modelInfo_.modelId_);
	}
}

// 解放
void Aitem::Release(void)
{
	MV1DeleteModel(aitemInfo_.modelInfo_.modelId_);
}

// アイテムを消す処理
void Aitem::Blaser(void)
{
	if (blaserTime_ < 0.0f)
	{
		aitemInfo_.modelInfo_.pos_ = player_->GetInfo().pos_;
		aitemInfo_.isShot_ = false;
	}
	else
	{
		blaserTime_ -= SceneManager::GetInstance().GetDeltaTime();
	}
}

void Aitem::SetTransformVec(VECTOR Normal)
{
	VECTOR normRefVec = VNorm(GetReflectVector(aitemInfo_.transformMoveVec_, Normal));
	aitemInfo_.transformMoveVec_ = VScale(normRefVec, REFLECT_SPEED);
}

void Aitem::SetRotation(VECTOR rot)
{
	MV1SetRotationXYZ(aitemInfo_.modelInfo_.modelId_, rot);
}
