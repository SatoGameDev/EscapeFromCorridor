#pragma once
#include <DxLib.h>

// クラスの前方宣言
class Player;			// プレイヤー

class Magatama
{
public:
	// 勾玉の最大数
	static constexpr int MAGATAMA_MAX_NUM = 3;	

	// 勾玉の初期サイズ
	static constexpr VECTOR MAGATAMA_SCALE = { 10.0f, 10.0f, 10.0f };

	// 仮の表示範囲
	static constexpr int MAP_NUM = 4;

	// 勾玉の構造体
	struct Info
	{
		int    modelId_;	 // モデルID
		VECTOR scale_;		 // 大きさ
		VECTOR rot_;		 // 回転値
		VECTOR pos_;		 // 座標
		bool   drawFlg_;	 // 表示フラグ
		bool   drawBillFlg_; // ビルボード表示フラグ
	};

	Magatama(void);				// コンストラクタ
	~Magatama(void);			// デストラクタ

	// 初期処理
	void Init(int _orgHandle);		// 初期化
	void Update(void);		// 更新
	void Draw(void);		// 描画
	void Release(void);		// 解放

	void DrawDebug(void);	// デバッグ

	//----------------------
	// ゲッター関数
	//----------------------
	int GetModelId(int index);	// モデルID
	VECTOR GetPos(int index);	// モデルの座標
	bool GetDrawFlg(int index); // モデルの表示フラグ

	//----------------------
	// セッター関数
	//----------------------
	bool SetDrawFlg(int index, bool _mFlg);		// モデル表示フラグ
	bool SetDrawBillFlg(int index, bool _mFlg);	// ビルボード表示フラグ

private:
	// 構造体変数の配列
	struct Info list[MAGATAMA_MAX_NUM];
	
	// 勾玉の座標の配列
	VECTOR mPos[MAGATAMA_MAX_NUM];

	// ビルボードで描画する画像
	int handle_;
};