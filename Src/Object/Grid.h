#pragma once
#include <DxLib.h>

class Grid
{
public:
	Grid(void);				// コンストラクタ
	~Grid(void);			// デストラクタ

	// 初期処理
	void Init(void);		// 初期化
	void Update(void);		// 更新
	void Draw(void);		// 描画
	void Release(void);		// 解放

private:
	// 線関連
	static constexpr float TERM = 600.0f;							// 間隔
	static constexpr float LINE_LEN = 12000.0f;						// 長さ
	static constexpr float H_LINE_LEN = LINE_LEN / 2.0f;			// 長さの半分
	static const int LINE_NUM = static_cast<int>(LINE_LEN / TERM);	// 本数
	static const int H_LINE_NUM = LINE_NUM / 2;						// 本数の半分
	static constexpr float CENTER_OFFSET = 300.0f;					// 各範囲の真ん中のオフセット
	
	// 球体関連
	static constexpr float SPHERE_RADIUS = 20.0f;					// 半径
	static const int SPHERE_DIV_NUM = 10;							// 形成するポリゴンの細かさ
	static constexpr float SPHERE_CENTER_RADIUS = 5.0f;				// 各範囲内に表示する用の半径
};