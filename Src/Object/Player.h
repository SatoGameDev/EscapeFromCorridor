#pragma once
#include <DxLib.h>
#include <functional>
#include <map>

// クラスの前方宣言
class Stage;
class Enemy;

class Player
{
public:
	// プレイヤーの移動状態管理用
	enum class STATE
	{
		NONE,
		IDLE,			// アイドル
		SNEAK,			// しゃがみ
		WALK,			// 歩き
		RUN,			// ダッシュ
		DEAD			// 死亡
	};

	// HP
	static constexpr int PLAYER_MAX_HP = 100;	// 最大のHP	
	
	// 移動量
	static constexpr float MOVE_SPEED_NONE  =  0.0f;		// 動いてない
	static constexpr float MOVE_SPEED_SNEAK =  3.5f;		// しゃがみ
	static constexpr float MOVE_SPEED_WALK  =  7.0f;		// 歩き
	static constexpr float MOVE_SPEED_RUN   = 10.0f;		// 走る

	// 頭の高さ
	static constexpr float MAX_HEAD_LINE = 170.0f;
	static constexpr float MIN_HEAD_LINE = 100.0f;
	static constexpr float MOVE_HEAD_SPEED = 5.0f;

	// 視点方向先
	static constexpr float BPOS_Z = 300.0f;

	// 初期モデル補正角度
	static constexpr VECTOR INIT_MODEL_ROT_OFFSET = { 0.0f, DX_PI_F, 0.0f };

	// カプセル構造体
	struct Capsule
	{
		VECTOR start;		// 始点
		VECTOR end;			// 終点
		float radius;		// 半径
	};

	// プレイヤー情報構造体
	struct Info
	{
		int modelId_;				// モデルID
		VECTOR scale_;				// サイズ
		VECTOR rot_;				// 角度
		VECTOR pos_;				// 座標
		VECTOR localRot_;			// 調整用初期角度
		VECTOR moveVecRad_;			// 移動ベクトルが生成する角度
		STATE moveKind_;			// プレイヤーの移動状態
		float moveSpeed_;			// プレイヤーの移動スピード
		int hp;						// 現在のHP
		bool aliveFlg_;				// 生存フラグ

		// 移動ベクトル
		VECTOR moveVec_;			// 移動ベクトル

		// マップ処理
		int posX;
		int xNum;
		int xRest;
		int xSet;

		int posZ;
		int zNum;
		int zRest;
		int zSet;

		VECTOR cPos_;			// カメラの座標(プレイヤーの頭の位置)
		VECTOR cAng_;			// カメラの角度
		VECTOR bPos_;			// 視点の先にある球体の座標
		VECTOR bcVec_;			// カメラ座標と球体座標の差分ベクトル
		float  pbDis;			// 差分ベクトルを用いて距離を計算する
		float  headLine;		// 頭の高さ

		struct Capsule cap;		// プレイヤーのカプセル
	};

	Player(void);			// コンストラクタ
	~Player(void);			// デストラクタ

	// 初期処理
	void Init(void);		// 初期化
	void Update(const int stageModel);		// 更新
	void Draw(void);		// 描画
	void Release(void);		// 解放

	// 更新
	void UpdateMove(const int stageModel);
	void UpdateMap(void);

	// デバッグ描画
	void DrawDebug(void);

	// ベクトルの移動判定
	bool IsMove(VECTOR _moveVec);

	// 勾玉を取得するための計算をする
	void ProcessMagaGet(void);

	// 通常の移動に戻る
	void NormalMove(void);

	// プレイヤーが死亡した時の処理
	void ProcessPlayerDead(void);

	// 当たり判定
	void Collision(const int stageModel);
	void CollToWall(const int stageModel);
	void CollToWall_another(const int stageModel);


	//----------------------
	// ゲッター関数
	//----------------------
	struct Info GetInfo(void){ return list; }
	float GetHeadPos(void) { return list.headLine; }

	//----------------------
	// セッター関数
	//----------------------
	void SetPlayerPos(const VECTOR& pos);		// プレイヤーの座標を設定する
	void SetRotation(void);						// 回転設定
	void SetDamage(int dp);						// プレイヤーのHPを減らす
	void SetPlayerSpeed(const VECTOR& speed) { list.moveVec_ = speed; }	// 速度設定

	
private:
	// 構造体変数
	struct Info list;
	
	//状態管理
	std::map<STATE, std::function<void(void)>> stateChange_;
	std::function<void(void)> stateUpdate_;

	VECTOR move_;			// 視点方向を加味した移動ベクトル

	float rad_;			// moveVec_ と move_ の角度
	float rad_half;		// rad_の半分
	float bcVec_rad;
	float move_rad;

	VECTOR moveVecNormEnd;		// moveVecの終点
	VECTOR moveNormEnd;			// moveの終点

	VECTOR subVec;
	float subVecSize;
	float subVecSize_half;
	VECTOR subHalfPos;

	VECTOR subHalfVec;
	float subHalfVecSize;

	// 経路探索
	int heuristic(int x1, int z1, int x2, int z2);	// ヒューリスティック関数（マンハッタン距離）

	// インスタンスのポインタ
	Enemy* enemy_;		// 敵

	void UpHead(void);
	void DownHead(void);
};
