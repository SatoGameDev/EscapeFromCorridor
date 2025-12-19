#pragma once
#include <DxLib.h>

// ラジアン(rad)・度(deg)変換用
static constexpr float RAD_TO_DEG = (180.0f / DX_PI_F);
static constexpr float DEG_TO_RAD = (DX_PI_F / 180.0f);

// 逆ベクトルを返す
VECTOR GetInverseVector(VECTOR vec);

// 反射ベクトルを返す
VECTOR GetReflectVector(VECTOR InVec, VECTOR NormalVec);

// 壁ずりベクトルを返す
VECTOR GetWallSlideVector(VECTOR InVec, VECTOR NormalVec);

// 角度ベクトル(ラジアンからデグリー)
VECTOR VRadToDeg(VECTOR vec);

// 角度ベクトル(デグリーからラジアン)
VECTOR VDegToRad(VECTOR vec);

