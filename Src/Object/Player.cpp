#include <DxLib.h>
#include <cmath>
#include "Player.h"
#include "Stage.h"
#include "../Common/Library.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/SoundManager.h"
#include "Enemy.h"

// コンストラクタ
Player::Player(void)
	: list({0}), enemy_(nullptr)
{
}

// デストラクタ
Player::~Player(void)
{
}

// 初期化
void Player::Init(void)
{
	// 敵
	enemy_ = new Enemy();

	//-----------------------------
	// 構造体変数の初期化
	//-----------------------------
	list.modelId_    = MV1LoadModel((Application::PATH_MODEL + 
			 						 "Human.mv1").c_str());	// モデルの読み込み
	list.scale_      = Application::DEFAULT_SCALE;			// 大きさ設定
	list.rot_	     = Application::DEFAULT_VECTOR;			// 角度設定
	list.pos_	     = { 900.0f, 0.0f, 300.0f };			// 初期位置設定
	list.localRot_   = INIT_MODEL_ROT_OFFSET;				// ローカル角度
	list.moveVecRad_ = Application::DEFAULT_VECTOR;			// 移動ベクトルが作成する角度
	list.moveKind_   = STATE::NONE;							// 移動状態
	list.moveSpeed_  = MOVE_SPEED_WALK;						// 移動速度
	list.hp			 = PLAYER_MAX_HP;						// HP
	list.aliveFlg_   = true;								// 生存フラグ
	list.moveVec_	 = Application::DEFAULT_VECTOR;			// 移動ベクトル

	list.posX  = 0;											// X : 座標をint型にする
	list.xNum  = 0;											//	   どの升目にいるか
	list.xRest = 0;											//     升目の中での座標
	list.xSet  = 0;											//     升目の中のどの升目にいるか
	list.posZ  = 0;											// Z : 座標をint型にする 
	list.zNum  = 0;											//	   どの升目にいるか 
	list.zRest = 0;											//     升目の中での座標 
	list.zSet  = 0;											//     升目の中のどの升目にいるか 

	list.cPos_  = Application::DEFAULT_VECTOR;				// カメラの座標(プレイヤーの頭の位置)
	list.cAng_  = Application::DEFAULT_VECTOR;				// カメラの角度
	list.bPos_  = Application::DEFAULT_VECTOR;				// 視点の先の座標
	list.bcVec_ = Application::DEFAULT_VECTOR;				// カメラ座標と視点先座標との差分ベクトル
	list.pbDis  = 0.0f;										// 差分ベクトルを用いて距離を計算する
	list.headLine = MAX_HEAD_LINE;

	list.cap = { 0 };

	//-----------------------------
	// 大きさ・回転・座標の設定
	//-----------------------------
	MV1SetScale		 (list.modelId_, list.scale_);	// 大きさ設定
	MV1SetRotationXYZ(list.modelId_, list.rot_);	// 角度設定
	MV1SetPosition	 (list.modelId_, list.pos_);	// 初期位置設

	// シングルトンの呼び出し
	SoundManager* seMana_ = SoundManager::GetInstance();
	// SEのロード
	// ゆっくり歩く音
	seMana_->LoadSE("Walk", (Application::PATH_SOUND + "SE/walk_floor_shoes1.mp3").c_str(),false);
	// 走る音
	seMana_->LoadSE("Run", (Application::PATH_SOUND + "SE/walk_floor_shoes2.mp3").c_str(),false);

	rad_half = 0.0f;
}

// 更新
void Player::Update(const int stageModel)
{
	list.cap = { list.pos_, list.cPos_, 50.0f };	// カプセルの更新

	UpdateMap();
	UpdateMove(stageModel);
	SetRotation();
	ProcessMagaGet();

	

	/*switch (moveKind_)
	{
	case STATE::NONE:
		moveSpeed_ = MOVE_SPEED_NONE;
		break;
	case STATE::IDLE:
		moveSpeed_ = MOVE_SPEED_NONE;
		break;
	case STATE::SNEAK:
		moveSpeed_ = MOVE_SPEED_SNEAK;
		break;
	case STATE::WALK:
		moveSpeed_ = MOVE_SPEED_WALK;
		break;
	case STATE::RUN:
		moveSpeed_ = MOVE_SPEED_RUN;
		break;
	case STATE::DEAD:
		moveSpeed_ = MOVE_SPEED_NONE;
		break;
	default:
		moveSpeed_ = MOVE_SPEED_NONE;
		break;
	}*/

	// シングルトンの呼び出し
	SoundManager* seMana_ = SoundManager::GetInstance();
	
	// リスナーの座標と向きの設定
	Set3DSoundListenerPosAndFrontPos_UpVecY(list.pos_, list.cAng_);
	// SEの座標の設定
	seMana_->SetPosSE("Walk", list.pos_);
	seMana_->SetPosSE("Run", list.pos_);
	// SEの聞こえる範囲の設定
	seMana_->SetAreaSE("Walk", 500.0f);
	seMana_->SetAreaSE("Run", 1000.0f);
	// SEの音量の設定
	seMana_->SetVolumeSE("Walk", 150);
	seMana_->SetVolumeSE("Run", 200);
}

// 描画
void Player::Draw(void)
{
	//// デバッグ------------------------------------------------------------------------------
	//DrawFormatString(20, 280, 0xffffff, "rad_ = %f", rad_);
	//DrawFormatString(20, 300, 0xffffff, "rad_half = %f", rad_half);
	//DrawFormatString(20, 320, 0xffffff, "bcVec_rad = %1.2lf", bcVec_rad);
	//DrawFormatString(20, 340, 0xffffff, "move_rad = %1.2lf", move_rad);
	//DrawFormatString(20, 360, 0xffffff, "moveVecToMove = %1.2lf, %1.2lf, %1.2lf", 
	//				 subVec.x, subVec.y, subVec.z);

	//DrawFormatString(20, 380, 0xffffff, "cAng_ = %1.2lf", list.cAng_.y);

	//DrawLine3D(list.pos_,   moveVecNormEnd, 0xffffff);
	//DrawLine3D(list.pos_,   subHalfPos,		0xff0000);
	//DrawLine3D(list.pos_,   moveNormEnd,	0xff00ff);
	//DrawLine3D(moveNormEnd, moveVecNormEnd, 0xffff00);
	////---------------------------------------------------------------------------------------

}

// 解放
void Player::Release(void)
{
	// モデルの解放
	MV1DeleteModel(list.modelId_);

	// シングルトンの呼び出し
	SoundManager* seMana_ = SoundManager::GetInstance();
	// SEの停止
	seMana_->StopSE("Walk");
	seMana_->StopSE("Run");
	seMana_->StopSE("Heart");
}

// 更新
void Player::UpdateMove(const int stageModel)
{
	// 入力制御取得
	InputManager& ins = InputManager::GetInstance();

	// シングルトンの呼び出し
	SoundManager* seMana_ = SoundManager::GetInstance();

	// WASDでプレイヤー移動
	list.moveVec_ = Application::DEFAULT_VECTOR;
	move_ = Application::DEFAULT_VECTOR;

	// 左・右・手前・奥のベクトルを作成する
	VECTOR RIGHT_MOVE_VEC = {  1.0f,  0.0f,  0.0f };
	VECTOR LEFT_MOVE_VEC  = { -1.0f,  0.0f,  0.0f };
	VECTOR FRONT_MOVE_VEC = {  0.0f,  0.0f,  1.0f };
	VECTOR BACK_MOVE_VEC  = {  0.0f,  0.0f, -1.0f };

	// 入力方向に移動ベクトルを追加する
	if (ins.IsNew(KEY_INPUT_W)) { list.moveVec_ = VAdd(list.moveVec_, FRONT_MOVE_VEC); }
	if (ins.IsNew(KEY_INPUT_A)) { list.moveVec_ = VAdd(list.moveVec_, LEFT_MOVE_VEC);  }
	if (ins.IsNew(KEY_INPUT_S)) { list.moveVec_ = VAdd(list.moveVec_, BACK_MOVE_VEC);  }
	if (ins.IsNew(KEY_INPUT_D)) { list.moveVec_ = VAdd(list.moveVec_, RIGHT_MOVE_VEC); }

	if (ins.IsNew(KEY_INPUT_W)) { move_ = VAdd(move_, FRONT_MOVE_VEC); }
	if (ins.IsNew(KEY_INPUT_A)) { move_ = VAdd(move_, LEFT_MOVE_VEC); }
	if (ins.IsNew(KEY_INPUT_S)) { move_ = VAdd(move_, BACK_MOVE_VEC); }
	if (ins.IsNew(KEY_INPUT_D)) { move_ = VAdd(move_, RIGHT_MOVE_VEC); }

	// プレイヤーとのマンハッタン距離を計算
	int distance = heuristic(enemy_->GetInfo().xNum, enemy_->GetInfo().zNum, list.xNum, list.zNum);

	// 距離が2以下の場合はプレイヤーに直接追従
	if (distance <= 5)
	{
		// プレイヤーの位置に向かうベクトルを計算
		VECTOR targetVec = VSub(enemy_->GetInfo().modelInfo_.pos_, list.pos_);

		enemy_->UpdatePos(targetVec);
	}

	
		

	// ベクトルの移動が行われていたら座標更新
	if (IsMove(list.moveVec_))
	{
		// 移動状態の設定
		list.moveKind_ = STATE::WALK;

		// カメラ角度分設定する
		VECTOR cameraAngles = SceneManager::GetInstance().GetCamera()->GetAngles();
		MATRIX cameraMatY = MGetRotY(cameraAngles.y);
		list.moveVec_ = VTransform(list.moveVec_, cameraMatY);
		move_ = VTransform(move_, cameraMatY);

		// スタミナが切れているかどうか
		bool spFlg_ = false;

		if (spFlg_)
		{
			// スタミナない状態
			list.moveSpeed_ = MOVE_SPEED_SNEAK;
			list.moveKind_ = STATE::SNEAK;
		}
		else
		{
			if (ins.IsNew(KEY_INPUT_LCONTROL))
			{
				// しゃがみ状態
				list.moveSpeed_ = MOVE_SPEED_SNEAK;
				list.moveKind_ = STATE::SNEAK;
				DownHead();

			}
			else
			{
				UpHead();

				if (ins.IsNew(KEY_INPUT_LSHIFT))
				{
					// ダッシュ状態
					list.moveSpeed_ = MOVE_SPEED_RUN;
					list.moveKind_ = STATE::RUN;
					// SEの再生
					seMana_->PlaySE("Run");
				}
				else
				{
					// 歩き状態
					list.moveSpeed_ = MOVE_SPEED_WALK;
					list.moveKind_ = STATE::WALK;
					// SEの再生
					seMana_->PlaySE("Walk");
				}
			}

		}

		// 座標更新
		Collision(stageModel);

		list.moveVec_ = VNorm(list.moveVec_);
		list.moveVec_ = VScale(list.moveVec_, list.moveSpeed_);
		list.pos_ = VAdd(list.pos_, list.moveVec_);

		

		// 方向を角度に変換する( XZ平面 Y軸)
		list.moveVecRad_.y = atan2f(list.moveVec_.x, list.moveVec_.z);

		// シンプルに計算角度を設定してみる
		list.rot_.y = list.moveVecRad_.y;

		// 座標設定
		MV1SetPosition(list.modelId_, list.pos_);
	}
	else
	{
		// 移動状態の設定
		list.moveKind_ = STATE::NONE;

		if (ins.IsNew(KEY_INPUT_LCONTROL))
		{
			DownHead();
		}
		else
		{
			UpHead();
		}

		// SEの停止
		seMana_->StopSE("Run");
		seMana_->StopSE("Walk");
	}
}

void Player::UpdateMap(void)
{
	list.posX  = static_cast<int>(list.pos_.x);	// 座標をint型にする
	list.xNum  = list.posX / 600;				// ワールド座標をマップ座標にする
	list.xRest = list.posX % 600;				// マップ座標内での座標(InMapPosと名付けておく)用に余りを計算しておく
	list.xSet  = list.xNum * 3;					// マップ座標をさらに分割し、マップ座標内でのマップ座標(InMapMap)を作る

	// InMapPos を InMapMap に変換する
	if (list.xRest <= 50)			
	{
		// InMapPos が 50 以下
		list.xSet += 0;
	}
	else if (list.xRest < 550)		
	{
		// InMapPos が 50 以上かつ 550 未満
		list.xSet += 1;
	}
	else						
	{
		// InMapPos が 550 以上
		list.xSet += 2;
	}

	list.posZ = static_cast<int>(list.pos_.z);	// 座標をint型にする
	list.zNum = list.posZ / 600;				// ワールド座標をマップ座標にする
	list.zRest = list.posZ % 600;				// マップ座標内での座標(InMapPosと名付けておく)用に余りを計算しておく
	list.zSet = list.zNum * 3;					// マップ座標をさらに分割し、マップ座標内でのマップ座標(InMapMap)を作る

	// InMapPos を InMapMap に変換する
	if (list.zRest <= 50)			
	{
		// InMapPos が 50 以下
		list.zSet += 0;
	}
	else if (list.zRest < 550)		
	{
		// InMapPos が 50 以上かつ 550 未満
		list.zSet += 1;
	}
	else						
	{
		// InMapPos が 550 以上
		list.zSet += 2;
	}
}

// デバッグ描画
void Player::DrawDebug(void)
{
	// モデルの描画
	MV1DrawModel(list.modelId_);

	// モデルの移動方向
	if (IsMove(list.moveVec_))
	{
		// 移動ベクトルを正規化
		VECTOR debugMoveVec = VNorm(list.moveVec_);

		// 線の長さを設定
		constexpr float DEBUG_MOVE_LINE_LENGTH = 100.0f;
		debugMoveVec = VScale(debugMoveVec, DEBUG_MOVE_LINE_LENGTH);

		// 線の終端座標を設定
		VECTOR debugMoveVecLineEndPos = VAdd(list.pos_, debugMoveVec);

		// 移動方向に線を描画
		DrawLine3D(list.pos_, debugMoveVecLineEndPos, 0xffffff);
	}

	/*switch (moveKind_)
	{
	case STATE::NONE:
		break;
	case STATE::IDLE:
		break;
	case STATE::SNEAK:
		DrawSphere3D(pos_, SPHERE_RADIUS_SNEAK, SPHERE_DIV_NUM, 0xff0000, 0xff0000, false);
		break;
	case STATE::WALK:
		DrawSphere3D(pos_, SPHERE_RADIUS_WALK, SPHERE_DIV_NUM, 0xff0000, 0xff0000, false);
		break;
	case STATE::RUN:
		DrawSphere3D(pos_, SPHERE_RADIUS_RUN, SPHERE_DIV_NUM, 0xff0000, 0xff0000, false);
		break;
	case STATE::DEAD:
		break;
	}*/


	DrawFormatString(20, 0, GetColor(0xff, 0xff, 0xff), 
				 	 "プレイヤーの座標 : (X, Y, Z) = (% 1.2lf, % 1.2lf, % 1.2lf)", 
					 list.pos_.x, list.pos_.y, list.pos_.z);

	DrawFormatString(20, 20, GetColor(0xff, 0xff, 0xff),
					 "移動スピード : %1.2lf", 
					 list.moveSpeed_);

	DrawFormatString(20, 40, GetColor(0xff, 0xff, 0xff), 
					 "カメラの座標 : % 1.2lf, % 1.2lf, % 1.2lf", 
					 list.cPos_.x, list.cPos_.y, list.cPos_.z);
	
	VECTOR cAng_ = VRadToDeg(list.cAng_);
	DrawFormatString(20, 60, GetColor(0xff, 0xff, 0xff), 
					 "カメラの角度 : % 1.2lf, % 1.2lf, % 1.2lf", 
					 cAng_.x, cAng_.y, cAng_.z);

	DrawFormatString(20, 80, GetColor(0xff, 0xff, 0xff),
				     "ボールの座標 : % 1.2lf, % 1.2lf, % 1.2lf",
					 list.bPos_.x, list.bPos_.y, list.bPos_.z);

	DrawFormatString(20, 100, GetColor(0xff, 0xff, 0xff),
					 "pbPos : % 1.2lf, % 1.2lf, % 1.2lf", 
					 list.bcVec_.x, list.bcVec_.y, list.bcVec_.z);

	DrawFormatString(20, 240, GetColor(0xff, 0xff, 0xff), 
					 "pbDis : % 1.2lf", 
					 list.pbDis);

	DrawFormatString(20, 260, GetColor(0xff, 0xff, 0xff), 
					 "pPos : % 1.2lf, % 1.2lf, % 1.2lf", 
					 list.pos_.x, list.pos_.y, list.pos_.z);

	float pbSize = VSize(list.bcVec_);
	DrawFormatString(20, 280, 0xffffff,
					 "pbSize : %1.2lf",
					 pbSize);
	
	DrawLine3D(list.cPos_, list.bPos_, GetColor(0, 255, 255));

	DrawSphere3D(list.bPos_, 20.0f, 20, 0x0000ff, 0x0000ff, false);

	DrawCapsule3D(list.cPos_, list.bPos_, 20.0f, 20, 0x0000ff, 0x0000ff, false);

}

// ベクトルの移動判定
bool Player::IsMove(VECTOR _moveVec)
{
	// XYZすべての座標の移動量の絶対値をとる
	float absX = abs(_moveVec.x);
	float absY = abs(_moveVec.y);
	float absZ = abs(_moveVec.z);

	// 移動ベクトルが X・Y・Z すべて移動されてなければ
	bool isNoMoveX = absX < FLT_EPSILON;
	bool isNoMoveY = absY < FLT_EPSILON;
	bool isNoMoveZ = absZ < FLT_EPSILON;

	// 座標更新せずに抜ける
	if (isNoMoveX && isNoMoveY && isNoMoveZ) return false;

	return true;
}

// 勾玉を取得するための計算をする
void Player::ProcessMagaGet(void)
{
	SceneManager& ins = SceneManager::GetInstance();

	// カメラの情報を取得
	list.cPos_ = ins.GetCamera()->GetPos();		// 座標
	list.cAng_ = ins.GetCamera()->GetAngles();	// 角度
	
	// 球体座標の計算-----------------------------------------------------------------------
	list.bPos_ = VAdd(list.cPos_, VGet(0.0f, 0.0f, BPOS_Z));

	float cAng_sinX = sinf(list.cAng_.x);		// 角度θの時のY軸の値を求めたい
	float cAng_sinY = sinf(list.cAng_.y);		// 角度θの時のX軸の値を求めたい
	float cAng_cosX = cosf(list.cAng_.x);		// 角度θの時のZ軸の値を求めたい(YZ平面)
	float cAng_cosY = cosf(list.cAng_.y);		// 角度θの時のZ軸の値を求めたい(XZ平面)

	// 射影長
	float pLen = cAng_cosX * BPOS_Z;
	// 角度θの時の高さ
	float posY = cAng_sinX * BPOS_Z * -1;

	// YZ平面
	list.bPos_.y = list.cPos_.y + posY;		// 現在の座標 + 高さ
	list.bPos_.z = list.cPos_.z + pLen;		// 現在の座標 + 射影長

	// XZ平面
	list.bPos_.x = list.cPos_.x + (cAng_sinY * pLen);
	list.bPos_.z = list.cPos_.z + (cAng_cosY * pLen);

	//-------------------------------------------------------------------------------------

	// カメラ座標と球体座標の差分ベクトル
	list.bcVec_ = VSub(list.bPos_, list.cPos_);

	// 差分ベクトルを用いて距離を計算する
	list.pbDis = VSize(list.bcVec_);
}

// 通常の移動に戻る
void Player::NormalMove(void)
{
	// プレイヤーの現在の移動状態に応じて移動速度を設定
	switch (list.moveKind_)
	{
	case STATE::NONE:
		list.moveSpeed_ = MOVE_SPEED_NONE;
		break;
	case STATE::IDLE:
		list.moveSpeed_ = MOVE_SPEED_NONE;
		break;
	case STATE::SNEAK:
		list.moveSpeed_ = MOVE_SPEED_SNEAK;
		break;
	case STATE::WALK:
		list.moveSpeed_ = MOVE_SPEED_WALK;
		break;
	case STATE::RUN:
		list.moveSpeed_ = MOVE_SPEED_RUN;
		break;
	case STATE::DEAD:
		list.moveSpeed_ = MOVE_SPEED_NONE;
		break;
	default:
		list.moveSpeed_ = MOVE_SPEED_NONE;
		break;
	}
}

// プレイヤーが死亡した時の処理
void Player::ProcessPlayerDead(void)
{
	// シーン遷移
	if (list.aliveFlg_ == false)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}
}

void Player::Collision(const int stageModel)
{
  //CollToWall(stageModel);
	CollToWall_another(stageModel);
}

// 当たり判定
void Player::CollToWall(const int stageModel)
{
	auto info = MV1CollCheck_Capsule(
		stageModel,
		-1,
		list.cap.start,
		list.cap.end,
		list.cap.radius
	);

	bcVec_rad = atan2f(list.bcVec_.x, list.bcVec_.z);
	move_rad = atan2f(move_.x, move_.z);	

	bool hitFlg = false;
	if (info.HitNum > 0) hitFlg = true;

	// プレイヤーが壁に衝突した場合
	if (hitFlg)
	{
		for (int i = 0; i < info.HitNum; i++)
		{
			auto hits = info.Dim[i];
			
			// 壁ずりベクトルを求める
			VECTOR wsVec_ = GetWallSlideVector(list.moveVec_, hits.Normal);

			// デバッグ用に移動ベクトルの終点を求める
			moveVecNormEnd = VAdd(list.pos_, VScale(VNorm(list.moveVec_), 100.0f));

			// デバッグ用に移動ベクトルの終点を求める
			moveNormEnd    = VAdd(list.pos_, VScale(VNorm(move_), 100.0f));	
			
			subVec = VSub(moveNormEnd, moveVecNormEnd);
			subVecSize_half = VSize(subVec) * 0.5f;


			subHalfPos = VAdd(moveVecNormEnd, VScale(VNorm(subVec), subVecSize_half));

			subHalfVec = VSub(subHalfPos, list.pos_);
			subHalfVecSize = VSize(subHalfVec);

			rad_half = atanf(subVecSize_half / subHalfVecSize) * Application::RAD_TO_DEG;
			rad_ = rad_half * 2.0f;


			// 左側の壁のみ正常に動作
			// 他の壁はそれぞれの角度の値などを用いて判定していくようにする
			if (rad_half <= 0.0f && move_rad <= 0.0f)
			{
				wsVec_.y = 0.0f;
				list.moveVec_ = wsVec_;
			}
		}
	}

	// 当たり判定結果ポリゴン配列の後始末をする
	MV1CollResultPolyDimTerminate(info);
}

// 当たり判定
void Player::CollToWall_another(const int stageModel)
{
	auto info = MV1CollCheck_Capsule(
		stageModel,
		-1,
		list.cap.start,
		list.cap.end,
		list.cap.radius
	);

	// プレイヤーが壁に衝突した場合
	if (info.HitNum > 0)
	{
		for (int i = 0; i < info.HitNum; i++)
		{
			auto hits = info.Dim[i];

			VECTOR N = VScale(VNorm(hits.Normal), (list.cap.radius + 1.0f));
			N.y = 0.0f;
			list.moveVec_ = VAdd(list.moveVec_, N);
		}
	}

	// 当たり判定結果ポリゴン配列の後始末をする
	MV1CollResultPolyDimTerminate(info);
}

//----------------------
// セッター関数
//----------------------
// プレイヤーの座標を設定する
void Player::SetPlayerPos(const VECTOR& pos)
{
	list.pos_ = pos;
	MV1SetPosition(list.modelId_, list.pos_);
}

// 回転設定
void Player::SetRotation(void)
{
	//------------------------------------
	// 回転行列を使用した角度設定
	//------------------------------------
	
	// 単位行列を設定する
	MATRIX mat = MGetIdent();

	// モデル自体のY軸回転行列を作成する
	MATRIX mGetRotY = MGetRotY(list.rot_.y);

	// モデルの補正用Y軸回転行列を作成する
	MATRIX mGetLocalRotY = MGetRotY(list.localRot_.y);

	// 行列を合成
	mat = MMult(mat, mGetRotY);
	mat = MMult(mat, mGetLocalRotY);

	// 行列を使用してモデルの角度を設定
	MV1SetRotationMatrix(list.modelId_, mat);
}

// プレイヤーのHPを減らす
void Player::SetDamage(int dp)
{
	list.hp -= dp;
	if (list.hp <= 0)
	{
		list.hp = 0;
		list.aliveFlg_ = false;
	}
}

// マンハッタン距離
int Player::heuristic(int x1, int z1, int x2, int z2)
{
	return abs(x1 - x2) + abs(z1 - z2);
}

void Player::UpHead(void)
{
	list.headLine += MOVE_HEAD_SPEED;
	if (list.headLine > MAX_HEAD_LINE)
	{
		list.headLine = MAX_HEAD_LINE;
	}
}

void Player::DownHead(void)
{
	list.headLine -= MOVE_HEAD_SPEED;
	if (list.headLine < MIN_HEAD_LINE)
	{
		list.headLine = MIN_HEAD_LINE;
	}
}
