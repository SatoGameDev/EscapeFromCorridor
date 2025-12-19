#pragma once
#include <DxLib.h>

// クラスの前方宣言
class Player;

class Light
{
public:
	// ライト情報
	struct Info
	{
		int lightHandle_;		// ハンドル
		VECTOR pos_;			// 座標
		VECTOR angle_;			// 向き
		float outAngle_;		// 影響角度(0.0f～DX_PI_F以下)
		float inAngle_;			// 影響角度内で減衰が始まる角度(0.0f～outAngle以下)
		float range_;			// 影響最大距離
		float Attan0_;			// 減衰率
		float Attan1_;			// 減衰率
		float Attan2_;			// 減衰率
	};

	Light();				// コンストラクタ
	~Light();				// デストラクタ

	// 初期処理
	void Init(void);			// 初期化
	void Update(void);			// 更新
	void Draw(void);			// 描画
	void Release(void);			// 解放

	void SetLightParam(void);	// ライトパラメータを設定
	void SetLightColor(void);	// ライトの色設定

	// セッター関数
	void SetPlayer(Player* pl) { player_ = pl; }

private:
	// 構造体変数
	struct Info list;

	// インスタンスのポインタ
	Player* player_;	// プレイヤー

};
