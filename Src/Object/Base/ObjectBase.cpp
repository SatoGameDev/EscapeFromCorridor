#include <DxLib.h>
#include <math.h>
#include "../../Manager/SceneManager.h"
#include "../../Manager/Camera.h"
#include "ObjectBase.h"

// コンストラクタ
ObjectBase::ObjectBase(void)
{
}

// デストラクタ
ObjectBase::~ObjectBase(void)
{
}

// 初期化
void ObjectBase::Init(void)
{
}

// 更新
void ObjectBase::Update(void)
{
}

// 描画
void ObjectBase::Draw(void)
{
}

// 解放
void ObjectBase::Release(void)
{
}

// 視点先の座標
VECTOR ObjectBase::LookPos(const float length)
{
	SceneManager& ins = SceneManager::GetInstance();

	// カメラの情報を取得
	VECTOR cPos_ = ins.GetCamera()->GetPos();
	VECTOR cAng_ = ins.GetCamera()->GetAngles();	// 角度

	// 球体座標の計算-----------------------------------------------------------------------
	VECTOR lookPos = VAdd(cPos_, VGet(0.0f, 0.0f, length));

	float cAng_sinX = sinf(cAng_.x);		// 角度θの時のY軸の値を求めたい
	float cAng_sinY = sinf(cAng_.y);		// 角度θの時のX軸の値を求めたい
	float cAng_cosX = cosf(cAng_.x);		// 角度θの時のZ軸の値を求めたい(YZ平面)
	float cAng_cosY = cosf(cAng_.y);		// 角度θの時のZ軸の値を求めたい(XZ平面)

	// 射影長
	float pLen = cAng_cosX * length;
	// 角度θの時の高さ
	float posY = cAng_sinX * length * -1;

	// YZ平面
	lookPos.y = cPos_.y + posY;		// 現在の座標 + 高さ
	lookPos.z = cPos_.z + pLen;		// 現在の座標 + 射影長

	// XZ平面
	lookPos.x = cPos_.x + (cAng_sinY * pLen);
	lookPos.z = cPos_.z + (cAng_cosY * pLen);

	//-------------------------------------------------------------------------------------

	return lookPos;
}
