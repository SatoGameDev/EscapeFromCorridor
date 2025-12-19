#include "Stage.h"
#include "../Application.h"
#include "../Manager/InputManager.h"

// コンストラクタ
Stage::Stage()
{
	list.torii_Info = { 0 };
}

// デストラクタ
Stage::~Stage()
{
}

// 初期化
void Stage::Init(void)
{
}

// 初期化
void Stage::Init(int stagemodel)
{
	//-----------------------------
	// 構造体変数の初期化
	//-----------------------------

	// 鳥居				       
	list.torii_Info.modelId_   = MV1LoadModel((Application::PATH_MODEL + "Stage/torii/torii.mv1").c_str());		 // モデルの読み込み
	list.torii_Info.scale_     = Application::DEFAULT_SCALE;												     // 大きさを設定
	list.torii_Info.rot_	   = { 0.0f, 45.0f * Application::DEG_TO_RAD, 0.0f };								 // 回転値を設定(Y軸90度回転)
	list.torii_Info.pos_	   = { 5600.0f, 0.0f, 5900.0f};														 // 位置を設定						

	// ワープ			       
	list.warp_Info.modelId_ = MV1LoadModel((Application::PATH_MODEL + "warp/warp.mv1").c_str());				 // モデルの読み込み
	list.warp_Info.scale_   = Application::DEFAULT_SCALE;														 // 大きさを設定
	list.warp_Info.rot_	    = Application::DEFAULT_VECTOR;														 // 回転値を設定(Y軸90度回転)
	list.warp_Info.pos_     = { 5780.0f, 120.0f, 5770.0f };														 // 位置を設定						

	//-----------------------------
	// 大きさ・回転・座標の設定
	//-----------------------------
	
	// 鳥居
	MV1SetScale		 (list.torii_Info.modelId_, list.torii_Info.scale_);	// 大きさを設定
	MV1SetRotationXYZ(list.torii_Info.modelId_, list.torii_Info.rot_);		// 回転値を設定
	MV1SetPosition	 (list.torii_Info.modelId_, list.torii_Info.pos_);		// 位置を設定

	// ワープ
	MV1SetScale		 (list.warp_Info.modelId_, list.warp_Info.scale_);		// 大きさを設定
	MV1SetRotationXYZ(list.warp_Info.modelId_, list.warp_Info.rot_);		// 回転値を設定
	MV1SetPosition	 (list.warp_Info.modelId_, list.warp_Info.pos_);		// 位置を設定

	// 当たり判定の設定
	MV1SetupCollInfo(stagemodel, -1, 1, 1, 1);
}

// 更新
void Stage::Update(void)
{
	list.warp_Info.rot_.z += 5.0f * Application::DEG_TO_RAD;
	list.warp_Info.rot_.y = 45.0f * Application::DEG_TO_RAD;
	if (list.warp_Info.rot_.z > 360.0f * Application::DEG_TO_RAD)
	{
		list.warp_Info.rot_.z = 0.0f;
	}

	// 単位行列を取得
	MATRIX mat = MGetIdent();

	// ZYの回転行列をそれぞれ取得
	MATRIX matRotZ = MGetRotZ(list.warp_Info.rot_.z);
	MATRIX matRotY = MGetRotY(list.warp_Info.rot_.y);

	// 単位行列に合成していく
	mat = MMult(mat, matRotZ);
	mat = MMult(mat, matRotY);

	MV1SetRotationMatrix(list.warp_Info.modelId_, mat);
}

// 描画
void Stage::Draw(void)
{
}

// 描画
void Stage::Draw(int _stage, int _floor, int _ceiling, int mNum)
{
	VECTOR scale_ = { 10.0f, 10.0f, 10.0f };
	VECTOR rot_ = { 1.57f, -1.57f, 0.0f };
	VECTOR pos_ = { 0.0f, 2.0f, 0.0f };

	MV1SetScale		 (_floor, scale_);		// 大きさを設定
	MV1SetRotationXYZ(_floor, rot_);		// 回転値を設定
	MV1SetPosition	 (_floor, pos_);		// 位置を設定


	MV1DrawModel(_floor);
	MV1DrawModel(_ceiling);
	MV1DrawModel(_stage);

	if (mNum >= 3)
	{
		MV1DrawModel(list.warp_Info.modelId_);
	}

	MV1DrawModel(list.torii_Info.modelId_);
}

// 解放
void Stage::Release(void)
{
	// ステージモデルの解放
	MV1DeleteModel(list.torii_Info.modelId_);
	MV1DeleteModel(list.warp_Info.modelId_);
}