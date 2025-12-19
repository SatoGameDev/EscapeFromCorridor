#pragma once
#include <DxLib.h>
#include <vector>
#include <queue>
#include <vector>
#include <cmath>
#include <algorithm>
#include <memory>
#include "Base/ObjectBase.h"
#include "Player.h"

// クラスの前方宣言
class Player;			// プレイヤー
class Aitem;

class Enemy : public ObjectBase
{
public:
	// マップ
	static constexpr int MAP_SIZE_WID = 600;		// １範囲の横サイズ
	static constexpr int MAP_SIZE_HIG = 600;		// １範囲の縦サイズ

	// 壁を含まないマップ(グリッド線に区切られた範囲)
	static constexpr int MAP_NUM_X = 10;			// 範囲の横数
	static constexpr int MAP_NUM_Z = 10;			// 範囲の縦数
	static constexpr int MAP_ALL_NUM = MAP_NUM_X * MAP_NUM_Z;
													// 総範囲数

	// 壁を含むマップ(グリッド線に区切られた範囲)
	static constexpr int EMAP_NUM_X = 30;			// 範囲の横数
	static constexpr int EMAP_NUM_Z = 30;			// 範囲の縦数
	static constexpr int EMAP_ALL_NUM = EMAP_NUM_X * EMAP_NUM_Z;
													// 総範囲数
	
	//  上下左右の移動ベクトル
	static constexpr VECTOR RIGHT_MOVE_VEC = { 1.0f,  0.0f,  0.0f };
	static constexpr VECTOR LEFT_MOVE_VEC = { -1.0f,  0.0f,  0.0f };
	static constexpr VECTOR FRONT_MOVE_VEC = { 0.0f,  0.0f,  1.0f };
	static constexpr VECTOR BACK_MOVE_VEC = { 0.0f,  0.0f, -1.0f };

	// エネミーの移動状態管理用
	enum class MOVE_ID
	{
		NONE,
		WALK,			// 歩き
		RUN				// 走る
	};

	// 移動量
	static constexpr float MOVE_SPEED_WALK = 2.5f;		// 歩き
	static constexpr float MOVE_SPEED_RUN =  8.0f;		// 走る

	// 初期モデル補正角度
	static constexpr VECTOR INIT_MODEL_ROT_OFFSET = { 0.0f, DX_PI_F / 2.0f, 0.0f };

	// 聴覚の広さ
	static constexpr float HEARING_RANGE = 1800.0f;

	// 向いている方向
	static constexpr VECTOR INIT_DIR = { 0.0f, 0.0f, -1.0f };

	// 視野角
	static constexpr float VIEW_ANGLE = 30.0f;

	// アニメーションの再生速度
	static constexpr float ANIM_SPEED = 0.2f;

	// カプセル構造体
	struct Capsule
	{
		VECTOR start;		// 始点
		VECTOR end;			// 終点
		float radius;		// 半径
	};

	// エネミー情報の構造体
	struct Info
	{
		Model modelInfo_;
		VECTOR localRot_;		// 調整用初期角度
		VECTOR moveVecRad_;		// 移動ベクトルが生成する角度
		MOVE_ID moveKind_;		// 移動状態
		float moveSpeed_;		// 移動スピード
		int moveDir_;			// 移動方向

		// 移動ベクトル
		VECTOR moveVec_;		// 移動ベクトル
		VECTOR oldMoveVec_;		// 直前の移動ベクトル

		// マップ処理
		int posX;
		int xNum;
		int xRest;
		int xSet;

		int posZ;
		int zNum;
		int zRest;
		int zSet;

		// 範囲内フラグ
		bool isNoticeHearing_;	// 聴覚
		bool isNoticeLooking_;	// 視野

		// アニメーション関連
		int animAttachNo_;		// 設定するアニメーションの番号
		float animTotalTime_;	// アニメーションの総再生時間
		float currentAnimTime_; // 直前のアニメーションの時間

		bool stanFlg_;			// アイテムがあたった時、数秒時間を止める

		struct Capsule cap;
	};
	


	// A*アルゴリズムで使用する構造体
	struct Node
	{
		int xNum;			// ノードのX座標
		int zNum;			// ノードのZ座標

		int startCost;		// 開始点からの移動コスト
		int hCost;			// ヒューリスティックコスト

		

		// 親ノード (経路を再構築するため)
		std::shared_ptr<Node> parent;

		// コンストラクタ : ノードの初期化
		Node(int x, int z, int g, int h, std::shared_ptr<Node> parent = nullptr)
			: xNum(x), zNum(z), startCost(g), hCost(h), parent(std::move(parent)) {}

		// f値(総コスト (f = g + h))
		int f() const { return startCost + hCost; }
	};


	// 優先度付きキューでノードを比較するための構造体
	struct CompareNode
	{
		bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b)
		{
			// fが小さいノードが優先される
			//if (a->f() <= b->f())
			//{
			//	return a->f();
			//}
			//else
			//{
			//	return b->f();
			//}


			 return b->f() > a->f();
		}
	};


	Enemy(void);					// コンストラクタ
	~Enemy(void) override;			// デストラクタ

	// 初期処理
	void Init(void) override;		// 初期化
	void Update(void) override;		// 更新
	void Update(int stagemodel);	// 更新
	void Draw(void) override;		// 描画
	void Release(void) override;	// 解放

	// 追加更新処理
	void UpdateMove(int stagemodel);		// 移動
	void UpdateSearch(void);	// 検知
	void UpdateRotation(void);	// 回転設定	
	void UpdatePos(VECTOR vec); // 座標更新

	// デバッグ描画
	void DrawDebug(void);

	// ベクトルの移動判定
	bool IsMove(VECTOR _moveVec);

	// 通常の移動に戻る
	void NormalMove();


	// マップ
	void InMapMapX(void);			// X座標がマップ内でどこにいるかの計算
	void InMapMapZ(void);			// Z座標がマップ内でどこにいるかの計算

	// 簡易的な移動処理
	void ProcessFront(void);		// 前に移動
	void ProcessBack(void);			// 後に移動
	void ProcessRight(void);		// 右に移動
	void ProcessLeft(void);			// 左に移動

	void IsCheckStan(void);

	//----------------------
	// ゲッター関数
	//----------------------
	struct Info GetInfo(void) { return list; }

	//----------------------
	// セッター関数
	//----------------------
	void SetEnemyPos(const VECTOR& pos);		// 敵の座標を設定する
	void SetEnemySpeed(const VECTOR& speed);	// 速度設定
	void SetPlayer(Player* player) { player_ = player; }// プレイヤーの情報を設定
	void SetStanFlg(bool flg) { list.stanFlg_ = flg; }

private:
	// 構造体を格納する変数
	struct Info list;

	// インスタンスのポインタ
	Player* player_;			// プレイヤー

	// 敵のスタン時間
	float stopTime;

	// デバッグ用
	static constexpr float SPHERE_RADIUS_WALK = 200.0f;					// 半径(歩き)
	static constexpr float SPHERE_RADIUS_RUN  = 400.0f;					// 半径(走る)
	static constexpr int   SPHERE_DIV_NUM     = 10;						// 形成するポリゴンの細かさ

	// マップの配列
	int eMap[EMAP_NUM_Z][EMAP_NUM_X] =
	{
		{1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1},
		{1,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,1},
		{1,0,0, 0,0,0, 0,0,1, 1,1,1, 1,0,1, 1,1,1, 1,0,0, 0,0,0, 0,0,1, 1,0,1},

		{1,0,0, 0,0,0, 0,0,1, 1,1,1, 1,0,1, 1,1,1, 1,0,0, 0,0,0, 0,0,1, 1,0,1},
		{1,0,0, 0,0,0, 0,0,1, 1,1,1, 1,0,1, 1,1,1, 1,0,0, 0,0,0, 0,0,1, 1,0,1},
		{1,0,1, 1,1,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,0,1},

		{1,0,1, 1,1,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,0,1},
		{1,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,0,1},
		{1,0,1, 1,1,1, 1,0,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,0,1},

		{1,0,1, 1,1,1, 1,0,1, 1,1,1, 1,0,0, 0,0,1, 1,1,1, 1,0,1, 1,1,1, 1,0,1},
		{1,0,1, 1,1,1, 1,0,1, 1,1,1, 1,0,0, 0,0,1, 1,1,1, 1,0,1, 1,1,1, 1,0,1},
		{1,0,1, 1,1,1, 1,0,1, 1,1,1, 1,0,0, 0,0,1, 1,1,1, 1,0,1, 1,1,1, 1,0,1},

		{1,0,1, 1,1,1, 1,0,1, 1,1,1, 1,0,0, 0,0,1, 1,1,1, 1,0,1, 1,1,1, 1,0,1},
		{1,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,1, 1,1,1, 1,0,1},
		{1,1,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,0,1},

		{1,1,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,0,1},
		{1,1,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,1,1, 1,1,1, 1,0,0, 0,0,0, 0,0,1},
		{1,1,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1},

		{1,1,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1},
		{1,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,1},
		{1,0,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,0,1, 1,0,1},

		{1,0,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,0,1, 1,0,1},
		{1,0,0, 0,0,0, 0,0,0, 0,0,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,0,1, 1,0,1},
		{1,0,0, 0,0,1, 1,1,1, 1,0,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,0,1, 1,0,1},

		{1,0,0, 0,0,1, 1,1,1, 1,0,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,0,1, 1,0,1},
		{1,0,0, 0,0,1, 1,1,1, 1,0,1, 1,1,1, 1,1,1, 1,0,0, 0,0,0, 0,0,1, 1,0,1},
		{1,0,0, 0,0,1, 1,1,1, 1,0,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,1,1, 1,0,1},

		{1,0,0, 0,0,1, 1,1,1, 1,0,1, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,1,1, 1,0,1},
		{1,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,1},
		{1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1}
	};

	// アニメーションの更新
	void UpdateAnimation(void);


	//----------------------------
	// 経路探索
	//----------------------------
	std::vector<std::pair<int, int>> path;		// 経路探索した結果を入れる動的二次元配列

	int  heuristic(int x1, int z1, int x2, int z2);	// ヒューリスティック関数（マンハッタン距離）
	void ChasePlayer(int stagemodel);		// 敵がプレイヤーを追跡する処理
	void NextPos(int index);	// 次の移動先を決定

	// A*アルゴリズムの実装
	std::vector<std::pair<int, int>> AStar(int startX, int startZ, int endX, int endZ, int eMap[EMAP_NUM_Z][EMAP_NUM_X]);

};

