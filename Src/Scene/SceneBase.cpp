#include "SceneBase.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"

// コンストラクタ
SceneBase::SceneBase(void)
{
}

// デストラクタ
SceneBase::~SceneBase(void)
{
}

// 初期化
void SceneBase::Init(int stagemodel)
{
}

// 更新
void SceneBase::Update(int stagemodel)
{
}

// 描画
void SceneBase::Draw(int stagemodel, int floormodel, int ceilingmodel)
{
}

// 解放
void SceneBase::Release(void)
{
}

bool SceneBase::IsCheckCursor(const int& posX, const int& posX2, const int& posY, const int& posY2)
{
	// 入力制御取得
	InputManager& ins = InputManager::GetInstance();

	// マウス座標の更新
	mousePos_ = ins.GetMousePos();

	if (mousePos_.x >= (float)posX && mousePos_.x <= (float)posX2 && 
		mousePos_.y >= (float)posY && mousePos_.y <= (float)posY2)
	{
		return true;
	}
	return false;
}

