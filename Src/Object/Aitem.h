#pragma once

#include "Base/ObjectBase.h"

// クラスの前方宣言
class Player;
class Stage;
class Enemy;

class Aitem : public ObjectBase
{
public:
	// 仮のスピード
	static constexpr float SPEED = 30.0f;
	static constexpr float REFLECT_SPEED = 15.0f;

	// 消えるまでの時間
	static constexpr float BLASER_TIME = 3.0f;

	// 重力
	static constexpr float GRAVITY = 0.1f;

	struct AITEM
	{
		Model modelInfo_;
		VECTOR oldPos_;				// 直前の座標
		VECTOR localPos_;			// ローカル座標
		VECTOR moveVec_;			// 移動ベクトル
		VECTOR oldMoveVec_;			// 直前の移動ベクトル
		VECTOR transformMoveVec_;
		float  gravityPower_;		// 重力
		float  time_;				// 消えるまでの時間
		bool   isShot_;				// 描画フラグ
	};

	Aitem(void);					// コンストラクタ
	~Aitem(void) override;			// デストラクタ

	// 初期処理
	void Init(void) override;		// 初期化
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void Release(void) override;	// 解放

	// アイテムを消す処理
	void Blaser(void);

	//----------------------
	// ゲッター関数
	//----------------------
	struct AITEM GetInfo(void) { return aitemInfo_; }

	//----------------------
	// セッター関数
	//----------------------
	void SetTransformVec(VECTOR Normal);
	void SetPlayer(Player* player) { player_ = player; }	// プレイヤーの情報を設定
	void SetRotation(VECTOR rot);
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }
	void SetStage(Stage* stage) { stage_ = stage; }

private:
	// 構造体
	AITEM aitemInfo_;

	// インスタンスのポインタ
	Player* player_;			// プレイヤー
	Stage* stage_;				// ステージ
	Enemy* enemy_;

	int box_;
	float blaserTime_;

};
