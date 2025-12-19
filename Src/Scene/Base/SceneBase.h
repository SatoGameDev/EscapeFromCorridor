#pragma once
#include <vector>
#include "../../Common/Vector2.h"

class SceneBase
{
public:
	SceneBase(void);					// コンストラクタ
	virtual ~SceneBase(void) = 0;		// デストラクタ

	// 初期処理
	virtual void Init(void)    = 0;		// 初期化
	virtual void Update(void)  = 0;		// 更新
	virtual void Draw(void)    = 0;		// 描画
	virtual void Release(void) = 0;		// 解放

protected:

	// テキストの構造体
	struct text
	{
		Vector2 textLeftUpPos_;		// 文字の左上の座標
		Vector2 textRightDownPos_;	// 文字の右上の座標
		int choicePic;			    // メニュー選択状態の画像
		int notChoicePic;			// メニュー非選択状態の画像
		int clickPic;				// メニュー決定時
		bool textFlg_;				// 文字選択フラグ
		bool clickFlg_;				// クリックしたかのフラグ
	};

	// マウスカーソルが範囲内にあるかどうか
	bool IsCheckCursor(const int& posX, const int& posX2, const int& posY, const int& posY2);

	// マウス座標
	Vector2 mousePos_;

	// 構造体の配列
	std::vector<text> uiDatalist;
};
