#include "Grid.h"

// コンストラクタ
Grid::Grid(void)
{
}

// コンストラクタ
Grid::~Grid(void)
{
}

// 初期化
void Grid::Init(void)
{
}

// 更新
void Grid::Update(void)
{
}

// 描画
void Grid::Draw(void)
{
	// 線座標
	VECTOR sPos = { 0.0f, 0.0f, 0.0f };	// 開始座標
	VECTOR ePos = { 0.0f, 0.0f, 0.0f };	// 終了座標
	VECTOR posX = { 0.0f, 0.0f, 0.0f }; // センタードット

	// グリッド線を引く
	for (int ii = 0; ii < LINE_NUM; ++ii)
	{
		// 線の番号
		float lineNumber = static_cast<float>(ii);

		// X軸(赤)→座標設定→線描画→球描画
		sPos = { 0.0f, 10.0f, lineNumber * TERM };
		ePos = { LINE_LEN, 10.0f, lineNumber * TERM };
		DrawLine3D(sPos, ePos, 0xff0000);
		DrawSphere3D(ePos, SPHERE_RADIUS, SPHERE_DIV_NUM, 0xff0000, 0xff0000, true);

		

		// Z軸(青)→座標設定→線描画→球描画
		sPos = { lineNumber * TERM, 10.0f, 0.0f };
		ePos = { lineNumber * TERM, 10.0f, LINE_LEN };
		DrawLine3D(sPos, ePos, 0x0000ff);
		DrawSphere3D(ePos, SPHERE_RADIUS, SPHERE_DIV_NUM, 0x0000ff, 0x0000ff, true);
		


		DrawLine3D(VGet(50.0f, 10.0f, 0.0f), VGet(50.0f, 10.0f, LINE_LEN), 0x00ffff);
		DrawLine3D(VGet(550.0f, 10.0f, 0.0f), VGet(550.0f, 10.0f, LINE_LEN), 0x00ffff);
		DrawLine3D(VGet(30.0f, 10.0f, 0.0f), VGet(30.0f, 10.0f, LINE_LEN), 0xff00ff);
		DrawLine3D(VGet(570.0f, 10.0f, 0.0f), VGet(570.0f, 10.0f, LINE_LEN), 0xff00ff);

		DrawLine3D(VGet(0.0f, 10.0f, 50.0f), VGet(LINE_LEN, 10.0f, 50.0f), 0xffff00);
		DrawLine3D(VGet(0.0f, 10.0f, 550.0f), VGet(LINE_LEN, 10.0f, 550.0f), 0xffff00);



		// 各範囲内に球を表示
		for (int zz = 0; zz < LINE_NUM; ++zz)
		{
			float zNum = static_cast<float>(zz);

			VECTOR posX = { lineNumber * TERM + CENTER_OFFSET, 10.0f, zNum * TERM + CENTER_OFFSET };
			DrawSphere3D(posX, SPHERE_CENTER_RADIUS, SPHERE_DIV_NUM, 0xffff00, 0xffff00, true);
		}
	}

	// Y軸(緑)(一本)→座標設定→線描画→球描画
	sPos = { 0.0f, -H_LINE_LEN, 0.0f };
	ePos = { 0.0f, H_LINE_LEN, 0.0f };
	DrawLine3D(sPos, ePos, 0x00ff00);
	DrawSphere3D(ePos, SPHERE_RADIUS, SPHERE_DIV_NUM, 0x00ff00, 0x00ff00, true);
}

// 解放
void Grid::Release(void)
{
}