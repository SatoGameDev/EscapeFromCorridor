#include <DxLib.h>
#include "../Application.h"

#include "../Manager/InputManager.h"
#include "../Object/Player.h"
#include "Camera.h"



// コンストラクタ
Camera::Camera(void)
{
	followPlayer_ = nullptr;

	pos_				= Application::DEFAULT_VECTOR;
	angles_				= Application::DEFAULT_VECTOR;
	localPosFormPlayer_ = Application::DEFAULT_VECTOR;
	screenCenter		= Application::DEFAULT_VECTOR;

	prevMousePos_		= { 0, 0 };
	nowMousePos_		= { 0, 0 };
}

// デストラクタ
Camera::~Camera(void)
{
}

// 初期化
void Camera::Init(void)
{
	// カメラの初期情報
	pos_	= Application::DEFAULT_VECTOR;		// 位置
	angles_ = Application::DEFAULT_VECTOR;		// 角度

	// 追従対象からのローカル座標設定
	localPosFormPlayer_ = { 0.0f, HEIGHT, -DIS_FOLLOW2CAMERA };

	// マウス座標
	prevMousePos_ = { 0, 0 };		// 直前
	nowMousePos_  = { 0, 0 };		// 現在

	screenCenter = { Application::HALF_SCREEN_SIZE_X,
					 Application::HALF_SCREEN_SIZE_Y,
					 0.0f };

}

// 更新
void Camera::Update(void)
{
}

// 描画
void Camera::Draw(void)
{	
}

// 解放
void Camera::Release(void)
{
	delete followPlayer_;
}

// 更新(GameScene)
void Camera::GameUpdate(void)
{
	// 入力制御取得
	InputManager& ins = InputManager::GetInstance();

	// クリップ距離を設定する
	SetCameraNearFar(5.0f, 30000.0f);

	// カメラの設定(位置と角度による制御)
	VECTOR UP_VECTOR = { 0.0f, 1.0f, 0.0f };
	VECTOR followPos = followPlayer_->GetInfo().pos_;
	VECTOR Localpos_ = VAdd(followPos, localPosFormPlayer_);
	SetCameraPositionAndTargetAndUpVec(
		Localpos_,
		followPos,
		UP_VECTOR
	);

	// マウス感度を設定
	float mouseSensi = 0.0008f;  // マウス感度の調整値


	// マウス座標
	prevMousePos_ = nowMousePos_;		// 直前
	nowMousePos_ = ins.GetMousePos();	// 現在

	// 画面の中央からカーソルまでの差分を計算
	float deltaX = nowMousePos_.x - screenCenter.x;
	float deltaY = nowMousePos_.y - screenCenter.y;

	// マウスの差分を使った回転制御
	angles_.y += deltaX * mouseSensi;  // 水平回転
	angles_.x += deltaY * mouseSensi;  // 垂直回転

	// ピッチ（X軸回転）の制限を行う（垂直方向のみ制限）
	if (angles_.x > MAX_PITCH) angles_.x = MAX_PITCH;		// 上方向の制限
	if (angles_.x < MIN_PITCH) angles_.x = MIN_PITCH;		// 下方向の制限

	// マウスを画面中央にリセット
	SetMousePoint((int)screenCenter.x, (int)screenCenter.y);

	// 単位行列を取得
	MATRIX mat = MGetIdent();

	// XYの回転行列をそれぞれ取得
	MATRIX matRotX = MGetRotX(angles_.x);
	MATRIX matRotY = MGetRotY(angles_.y);

	// 単位行列に合成していく
	mat = MMult(mat, matRotX);
	mat = MMult(mat, matRotY);

	// ローカル座標の回転
	//VECTOR localPos = VTransform(localPosFormPlayer_, mat);
	VECTOR localPos_ = { 0.0f, followPlayer_->GetHeadPos(), 0.0f};

	// 追従対象のローカル座標を追加してワールド座標にする
	pos_ = VAdd(followPos, localPos_);

	// カメラの位置と角度を設定
	SetCameraPositionAndAngle(
		pos_,
		angles_.x,  // 垂直回転
		angles_.y,  // 水平回転
		angles_.z   // Z軸の回転（必要であれば）
	);
}

// カメラ設定（毎フレーム実行）
void Camera::SetBeforeDraw(void)
{
	// クリップ距離を設定する
	SetCameraNearFar(10.0f, 30000.0f);

	// カメラの設定（位置と角度による制御）
	SetCameraPositionAndAngle(
		pos_,
		angles_.x,
		angles_.y,
		angles_.z
	);

}

// 追従するプレイヤーの設定
void Camera::SetPlayer(Player* _player)
{
	followPlayer_ = _player;
}

// カメラを初期に戻す
void Camera::ReSet(void)
{
	// カメラの初期情報
	pos_	= Application::DEFAULT_VECTOR;		// 位置
	angles_ = Application::DEFAULT_VECTOR;		// 角度
}