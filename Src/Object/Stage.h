#pragma once
#include <DxLib.h>
#include "Base/ObjectBase.h"

class Stage : public ObjectBase
{
public:
	// 升目の大きさ
	static constexpr float MAP_SIZE_X = 600.0f;		// １範囲の横サイズ
	static constexpr float MAP_SIZE_Z = 600.0f;		// １範囲の縦サイズ

	// 升目の数
	static constexpr int SQUARE_NUM_X = 10;							   // 横
	static constexpr int SQUARE_NUM_Z = 10;							   // 縦
	static constexpr int SQUARE_ALL_NUM = SQUARE_NUM_X * SQUARE_NUM_Z; // 総数	

	// マップの数
	static constexpr int MAP_NUM_X = SQUARE_NUM_X * 3;		 		   // 横
	static constexpr int MAP_NUM_Z = SQUARE_NUM_Z * 3;				   // 縦
	static constexpr int MAP_ALL_NUM = MAP_NUM_X * MAP_NUM_Z;		   // 総数

	struct Info
	{
		Model torii_Info;
		Model warp_Info;
	};

	Stage(void);					// コンストラクタ
	~Stage(void) override;			// デストラクタ

	// 初期処理
	void Init(void) override;		// 初期化
	void Init(int stagemodel);		// 初期化
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void Draw(int _stage, int _floor, int _ceiling, int mNum);	// 描画
	void Release(void);				// 解放

	//----------------------
	// ゲッター関数
	//----------------------
	struct Info GetInfo(void) { return list; }


private:	
	// 構造体変数
	struct Info list;
};
