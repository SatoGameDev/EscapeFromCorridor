#pragma once

// クラスの前方宣言
class Camera;		// カメラ
class Grid;			// グリッド線
class Stage;		// ステージ
class Player;		// プレイヤー
class Enemy;		// エネミー
class Magatama;		// 勾玉
class Aitem;		// アイテム
class Light;		// ライト

class Game : public SceneBase
{
public:
	Game(void);						// コンストラクタ
	~Game(void) override;			// デストラクタ

	// 初期処理
	void Init(void) override;		// 初期化
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void Release(void) override;	// 解放

	// デバッグ描画
	void DrawDebug(void);

	// 当たり判定
	void Collision(int stagemodel);			// まとめ
	void CollPlayerToEnemy(void);			// プレイヤーと　敵の当たり判定
	void CollLockCapToMagatama(void);		// 視線カプセルと勾玉の当たり判定
	void CollAitemToWall(int stagemodel);	// アイテムと　　壁の当たり判定
	void CollAitemToEnemy(void);			// アイテムと　　敵の当たり判定

private:
	// 画像のハンドルID
	int reticle_;				// レティクル
	int keyhelp_;				// キーヘルプ

	// インスタンスのポインタ
	Camera*   camera_;			// カメラ
	Grid*     grid_;			// グリッド線
	Stage*    stage_;			// ステージ
	Player*   player_;			// プレイヤー
	Enemy*    enemy_;			// エネミー
	Magatama* mtama_;			// 勾玉
	Aitem*    aitem_;			// アイテム
	Light*    light_;			// ライト


	// 勾玉の取得数
	int mNum_;					// 取得数
	int modelOriginalHandle_;	// 複製元ハンドル

	double rot;
	int warp;
	int letsget;
	int gotorii;
	int maga[4];
	int magatama;

	int stagemodel;
	int floormodel;
	int ceilingmodel;

	// シャドウマップハンドル
	int shadowMapHandle_;

};