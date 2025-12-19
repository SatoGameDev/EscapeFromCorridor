#pragma once

class ObjectBase
{
public:
	ObjectBase(void);					// コンストラクタ
	virtual ~ObjectBase(void) = 0;		// デストラクタ

	// 初期処理
	virtual void Init(void) = 0;		// 初期化
	virtual void Update(void) = 0;		// 更新
	virtual void Draw(void) = 0;		// 描画
	virtual void Release(void) = 0;		// 解放

protected:
	// オブジェクト構造体
	struct Model
	{
		int modelId_;	// モデルID
		VECTOR scale_;	// 拡大値
		VECTOR rot_; 	// 角度
		VECTOR pos_; 	// 座標
	};

	Model modelInfo;

	// 視点先の座標
	VECTOR LookPos(const float length);
};