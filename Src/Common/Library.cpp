#include "Library.h"

/// <summary>
/// 逆ベクトルを返す
/// </summary>
/// <param name="vec"> 符号を逆にしたいベクトル </param>
/// <returns>		   逆ベクトル				</returns>
VECTOR GetInverseVector(VECTOR vec)
{
	return { vec.x * -1, vec.y * -1, vec.z * -1 };
}

/// <summary>
/// 反射ベクトルを返す
/// </summary>
/// <param name="InVec">     F : 入射ベクトル </param>
/// <param name="NormalVec"> N : 法線ベクトル </param>
/// <param name="friction">		 摩擦		　</param>
/// <returns>				 R : 反射ベクトル </returns>
VECTOR GetReflectVector(VECTOR InVec, VECTOR NormalVec)
{
	// R = F - 2(F・N)N
	VECTOR R = VSub(InVec, VScale(NormalVec, (VDot(InVec, NormalVec) * 2.0f)));
	return R;
}

/// <summary>
/// 壁ずりベクトルを返す
/// </summary>
/// <param name="InVec">     F : 入射ベクトル   </param>
/// <param name="NormalVec"> N : 法線ベクトル   </param>
/// <returns>				 W : 壁ずりベクトル </returns>
VECTOR GetWallSlideVector(VECTOR InVec, VECTOR NormalVec)
{
	// W = F - (F・N)N
	VECTOR W = VSub(InVec, VScale(NormalVec, VDot(InVec, NormalVec)));
	return W;
}

// 角度ベクトル(ラジアンからデグリー)
VECTOR VRadToDeg(VECTOR vec)
{
	return { vec.x * RAD_TO_DEG, vec.y * RAD_TO_DEG, vec.z * RAD_TO_DEG };
}

// 角度ベクトル(デグリーからラジアン)
VECTOR VDegToRad(VECTOR vec)
{
	return  { vec.x * DEG_TO_RAD, vec.y * DEG_TO_RAD, vec.z * DEG_TO_RAD };
}
