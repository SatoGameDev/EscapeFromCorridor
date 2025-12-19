#pragma once
#include <chrono>

// クラスの前方宣言
class SceneBase;			// シーンベース
class Fader;				// フェーダー
class Camera;				// カメラ

class SceneManager
{

public:	

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,				// タイトル
		KEYHELP,			// キーヘルプ
		GAME,				// ゲーム
		GAMEOVER,			// ゲームオーバー
		GAMECLEAR			// ゲームクリア
	};

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	// 初期処理
	void Init(void);// 初期化
	void Init3D(void);// 初期化(3D)
	void Update(void);// 更新
	void Draw(void);// 描画
	void Release(void);// 解放

	// 状態遷移
	void ChangeScene(SCENE_ID nextId);

	// シーンIDの取得
	SCENE_ID GetSceneID(void);

	// デルタタイムの取得
	// return 1.0f / 60.0f
	float GetDeltaTime(void) const;

	// カメラの取得
	Camera* GetCamera(void) const;

	int GetWallModelId(void) const { return stageModel; }
	int GetFloorModelId(void) const { return floorModel; }
	int GetCeilingModelId(void) const { return ceilingModel; }

private:

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager(void);
	// コピーコンストラクタも同様
	SceneManager(const SceneManager&);
	// デストラクタも同様
	~SceneManager(void);

	// 静的インスタンス
	static SceneManager* instance_;

	// インスタンスのポインタ
	SceneBase* scene_;					// 各種シーン
	Fader* fader_;						// フェード
	Camera* camera_;					// カメラ

	// シーンID
	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	// シーン遷移中判定
	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	

	// デルタタイムをリセットする
	void ResetDeltaTime(void);

	// シーン遷移
	void DoChangeScene(SCENE_ID sceneId);

	// フェード
	void Fade(void);

	// GameSceneかどうか
	bool gameFlg_;

	// ステージモデル
	int stageModel;
	int floorModel;
	int ceilingModel;
};