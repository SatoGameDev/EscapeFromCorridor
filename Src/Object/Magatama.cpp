#include "Magatama.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "Player.h"

// コンストラクタ
Magatama::Magatama(void)
{
	for (int num = 0; num < MAGATAMA_MAX_NUM; num++)
	{
		list[num] = { 0 };
	}
}

// デストラクタ
Magatama::~Magatama(void)
{
}

// 初期化
void Magatama::Init(int _orgHandle)
{
	// ビルボード画像の読み込み
	handle_ = LoadGraph((Application::PATH_IMAGE + "FKey.png").c_str());

	// 勾玉の座標を設定
	mPos[0] = { 5100.0f, 80.0f, 900.0f  };
	mPos[1] = { 2700.0f, 80.0f, 2100.0f };
	mPos[2] = { 300.0f,  80.0f, 5100.0f };

	// 勾玉の構造体の初期化
	for (int ii = 0; ii < MAGATAMA_MAX_NUM; ii++)
	{
		//-----------------------------
		// 構造体変数の初期化
		//-----------------------------
		list[ii].modelId_	  = MV1DuplicateModel(_orgHandle);	// モデルの複成
		list[ii].scale_		  = MAGATAMA_SCALE;					// 大きさを設定
		list[ii].rot_		  = { 1.57f, -1.75f,  0.0f };		// 回転値を設定
		list[ii].pos_		  = mPos[ii];						// 位置を設定
		list[ii].drawFlg_	  = true;							// 描画フラグを設定
		list[ii].drawBillFlg_ = false;							// ビルボード描画フラグを設定

		//-----------------------------
		// 大きさ・回転・座標の設定
		//-----------------------------
		MV1SetScale		 (list[ii].modelId_, list[ii].scale_);	// 大きさを設定
		MV1SetRotationXYZ(list[ii].modelId_, list[ii].rot_);	// 回転値を設定
		MV1SetPosition	 (list[ii].modelId_, list[ii].pos_);	// 位置を設定
		MV1SetMaterialEmiColor(list[ii].modelId_, 0, GetColorF(0.0f, 0.5f, 0.0f, 0.5f));
	}
}

// 更新
void Magatama::Update(void)
{
}

// 描画
void Magatama::Draw(void)
{
	for (int ii = 0; ii < MAGATAMA_MAX_NUM; ii++)
	{
		if (list[ii].drawFlg_ == true)
		{
			// モデルの描画
			MV1DrawModel(list[ii].modelId_);

			if (list[ii].drawBillFlg_ == true)
			{
				// ビルボードの描画
				VECTOR fPos_ = VAdd(list[ii].pos_, VGet(0.0f, 20.0f, 0.0f));
				DrawBillboard3D(fPos_, 0.5f, 0.5f, 50.0f, 0.0f, handle_, true);

			}
		}
	}
}

// 解放
void Magatama::Release(void)
{
	// 勾玉モデルの解放
	for (int ii = 0; ii < MAGATAMA_MAX_NUM; ii++)
	{
		MV1DeleteModel(list[ii].modelId_);
	}
}
// デバッグ
void Magatama::DrawDebug(void)
{
	// 勾玉モデルの座標を描画
	for (int ii = 0; ii < MAGATAMA_MAX_NUM; ii++)
	{
		DrawFormatString(20, 120 + (ii * 20), GetColor(0xff, 0xff, 0xff), "勾玉の座標 : (X, Y, Z) = (% 1.2lf, % 1.2lf, % 1.2lf)",
			list[ii].pos_.x, list[ii].pos_.y, list[ii].pos_.z);

		DrawFormatString(20, 180 + (ii * 20), GetColor(0xff, 0xff, 0xff), "勾玉の表示フラグ : %d",
			list[ii].drawFlg_);
	}
}

// モデルIDのゲッター
int Magatama::GetModelId(int index)
{
	return list[index].modelId_;
}

// モデルの座標のゲッター
VECTOR Magatama::GetPos(int index)
{
	return list[index].pos_;
}

bool Magatama::GetDrawFlg(int index)
{
	return list[index].drawFlg_;
}

// モデル表示フラグのセッター
bool Magatama::SetDrawFlg(int index, bool _mFlg)
{
	return list[index].drawFlg_ = _mFlg;
}

// ビルボード表示フラグのセッター
bool Magatama::SetDrawBillFlg(int index, bool _mFlg)
{
	return list[index].drawBillFlg_ = _mFlg;
}

