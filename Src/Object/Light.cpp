#include "Light.h"
#include "../Manager/InputManager.h"
#include "../Application.h"
#include "Player.h"
#include "Enemy.h"

// コンストラクタ
Light::Light()
	: list({0}), player_(nullptr)
{
}

// デストラクタ
Light::~Light()
{
}

// 初期化
void Light::Init(void)
{
	player_ = new Player();

	//-----------------------------
	// 構造体変数の初期化
	//-----------------------------
	list.lightHandle_ = 0;
	list.pos_         =  player_->GetInfo().pos_;			// 位置
	list.angle_       = player_->GetInfo().bcVec_;			// 向き
	list.outAngle_    = 30.0f / Application::RAD_TO_DEG;	// 影響角度
	list.inAngle_     = 25.0f / Application::RAD_TO_DEG;	// 減衰角度
	list.range_       = 20000.0f;							// 有効距離

	// d = ライトから頂点への距離
	// ライトの影響力(％) = 100.0f / (Atten0 + Atten1 * d + Atten2 * d * d)
	list.Attan0_  = 0.235f;								// 減衰率
	list.Attan1_  = 0.00065f;							// 減衰率
	list.Attan2_  = 0.0f;								// 減衰率

	// 標準ライトをスポットライトに変更する
	list.lightHandle_ = ChangeLightTypeSpot(
		list.pos_,
		list.angle_,
		list.outAngle_,
		list.inAngle_,
		list.range_,
		list.Attan0_,
		list.Attan1_,
		list.Attan2_
	);

	
	// ライトパラメータの設定
	SetLightParam();

	// ライトカラー設定
	SetLightColor();
}

// 更新
void Light::Update(void)
{
	// 入力制御取得
	InputManager& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_R))
	{
		bool lightFlg = GetLightEnable();

		if (lightFlg)
		{
			SetLightEnable(false);
		}
		else
		{
			SetLightEnable(true);
		}
		
	}

	// 位置の更新
	list.pos_ = player_->GetInfo().cPos_;

	// 方向の更新
	list.angle_ = player_->GetInfo().bcVec_;

	// パラメータの更新
	SetLightParam();	
}

// 描画
void Light::Draw(void)
{
}

// 解放
void Light::Release(void)
{
}

// // ライトパラメータを設定
void Light::SetLightParam(void)
{
	// 方向の設定
	SetLightDirection(list.angle_);

	// 位置の設定
	SetLightPosition(list.pos_);

	// 有効距離、減衰率の設定
	SetLightRangeAtten(list.range_,
					   list.Attan0_, list.Attan1_, list.Attan2_);

	// 照射角度の設定
	SetLightAngle(list.outAngle_, list.inAngle_);
}

void Light::SetLightColor(void)
{
	// ディフューズカラー設定
	SetLightDifColor(GetColorF(1.0f, 1.0f, 1.0f, 1.0f));

	// スペキュラカラー設定
	SetLightSpcColor(GetColorF(1.0f, 1.0f, 1.0f, 1.0f));

	// アンビエントカラー設定
	SetLightAmbColor(GetColorF(0.3f, 0.3f, 0.3f, 0.3f));
}
	

	
