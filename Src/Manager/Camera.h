#pragma once
#include "../Common/Vector2.h"

class Player;			// プレイヤー

class Camera
{
public:
	// 角度の制限（特にX軸の垂直方向の回転を制限)
	const float MAX_PITCH =  DX_PI_F / 2.0f - 0.01f;		// +90度まで
	const float MIN_PITCH = -DX_PI_F / 2.0f + 0.01f;		// -90度まで

	// カメラの高さ
	static constexpr float HEIGHT = 200.0f;

	// 追従対象からカメラまでの距離
	static constexpr float DIS_FOLLOW2CAMERA = 500.0f;


	// コンストラクタ
	Camera(void);	

	// デストラクタ
	~Camera(void);					

	// 初期処理
	void Init(void);				// 初期化
	void Update(void);				// 更新
	void Draw(void);				// 描画
	void Release(void);				// 解放


	// 更新(GameSceneのみ)
	void GameUpdate(void);			

	// カメラ設定（毎フレーム実行）
	void SetBeforeDraw(void);

	// 追従するプレイヤーの設定
	void SetPlayer(Player* _player);
	
	// カメラを初期に戻す
	void ReSet(void);

	// カメラの座標
	VECTOR GetPos(void) { return pos_; }

	// カメラの角度
	VECTOR GetAngles(void) { return angles_; }


private:
	// インスタンスのポインタ
	Player* followPlayer_;			// 追従するプレイヤー

	// ベクター
	VECTOR pos_;					// 位置
	VECTOR localPosFormPlayer_;		// 追従対象からのローカル座標
	VECTOR angles_;					// 角度(rad)

	// カーソルの位置
	Vector2 prevMousePos_;			// 直前
	Vector2 nowMousePos_;			// 現在


	// 画面の中央座標（マウスをここにリセットする）
	VECTOR screenCenter;

};

