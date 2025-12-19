#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"
#include "Player.h"
#include "Enemy.h"

// コンストラクタ
Enemy::Enemy(void)
	: list({0}), player_(nullptr)
{
}

// デストラクタ
Enemy::~Enemy(void)
{
}

// 初期化
void Enemy::Init(void)
{
	// プレイヤー
	player_ = new Player();

	//-----------------------------
	// 構造体変数の初期化
	//-----------------------------
	list.modelInfo_.modelId_ = MV1LoadModel((Application::PATH_MODEL + "Enemy/enemy.mv1").c_str());	// モデルの読み込み
	list.modelInfo_.scale_ = { 10.0f, 10.0f, 10.0f };					// モデルの大きさを設定
	list.modelInfo_.rot_   = Application::DEFAULT_VECTOR;				// 角度設定
	list.modelInfo_.pos_   = { 3900.0f, 0.0f, 5100.0f };				// 初期位置設定
	list.localRot_   = INIT_MODEL_ROT_OFFSET;					// ローカル回転値
	list.moveVecRad_ = Application::DEFAULT_VECTOR;			// 移動ベクトルが作成する角度
	list.moveKind_   = MOVE_ID::NONE;							// 移動状態
	list.moveSpeed_  = MOVE_SPEED_WALK;						// 移動速度
	list.moveDir_    = 0;										// 移動方向
	list.moveVec_    = Application::DEFAULT_VECTOR;			// 移動ベクトル
	list.oldMoveVec_ = Application::DEFAULT_VECTOR;			// 直前の移動ベクトル
	list.posX        = 0;											// X : 座標をint型にする
	list.xNum		 = 0;											//	   どの升目にいるか
	list.xRest		 = 0;											//     升目の中での座標
	list.xSet		 = 0;											//     升目の中のどの升目にいるか
	list.posZ		 = 0;											// Z : 座標をint型にする 
	list.zNum		 = 0;											//	   どの升目にいるか 
	list.zRest		 = 0;											//     升目の中での座標 
	list.zSet		 = 0;											//     升目の中のどの升目にいるか 
	list.isNoticeHearing_ = false;							// 聴覚範囲内フラグ
	list.isNoticeLooking_ = false;							// 視覚範囲内フラグ
	list.animAttachNo_ = MV1AttachAnim(list.modelInfo_.modelId_, 1);	// 設定するアニメーションの番号
	list.animTotalTime_ = MV1GetAttachAnimTotalTime(		// アニメーションの総再生時間
							list.modelInfo_.modelId_,
							list.animAttachNo_);
	list.currentAnimTime_ = 0.0f;							// 直前のアニメーションの時間
	list.stanFlg_ = false;
	list.cap = { 0 };

	//-----------------------------
	// 大きさ・回転・座標の設定
	//-----------------------------
	MV1SetScale		 (list.modelInfo_.modelId_, list.modelInfo_.scale_);  // モデルの大きさを設定
	MV1SetRotationXYZ(list.modelInfo_.modelId_, list.modelInfo_.rot_);	// エネミーの角度設定
	MV1SetPosition	 (list.modelInfo_.modelId_, list.modelInfo_.pos_);	// エネミーの初期位置設定

	// アニメーション関連
	MV1SetAttachAnimTime(list.modelInfo_.modelId_, list.animAttachNo_, list.currentAnimTime_);

	// シングルトンの呼び出し
	SoundManager* seMana_ = SoundManager::GetInstance();

	// SEのロード
	// 敵の声
	seMana_->LoadSE("voice", (Application::PATH_SOUND + "SE/enemy_voice.mp3").c_str());

	// 心臓の音
	seMana_->LoadSE("Heart", (Application::PATH_SOUND + "SE/heartbeats.mp3").c_str(), false);

	// 敵に見つかった時のBGM
	seMana_->LoadBGM("EnemyBGM", (Application::PATH_SOUND + "BGM/enemybgm.mp3").c_str(), false);

	stopTime = 0.0f;
}

// 更新
void Enemy::Update(void)
{
}

// 更新
void Enemy::Update(int stagemodel)
{
	list.cap = { list.modelInfo_.pos_, VAdd(list.modelInfo_.pos_, VGet(0.0f, 170.0f, 0.0f)), 50.0f };

	// マップ処理
	InMapMapX();
	InMapMapZ();
	
	// 移動処理
	if (!list.stanFlg_)
	{
		stopTime = 3.0f;
		UpdateMove(stagemodel);
	}
	IsCheckStan();

	// アニメーション処理
	UpdateAnimation();

	// 回転処理
	UpdateRotation();

	// 検知処理
	//UpdateSearch();
	list.isNoticeHearing_ = list.isNoticeLooking_ = true;

	// シングルトンの呼び出し
	SoundManager* seMana_ = SoundManager::GetInstance();

	// SEの座標位置に設定
	seMana_->SetPosSE("voice", list.modelInfo_.pos_);
	seMana_->SetPosSE("Heart", list.modelInfo_.pos_);

	// SEの聞こえる範囲の設定
	seMana_->SetAreaSE("voice", 3200.0f);
	seMana_->SetAreaSE("Heart", 3200.0f);

	// SEの音量の設定
	seMana_->SetVolumeSE("voice", 255);
	seMana_->SetVolumeSE("Heart", 255);
	seMana_->SetVolumeSE("EnemyBGM", 255);

	// リバーブエフェクトの設定
	seMana_->SetReverEfectSE("voice", DX_REVERB_PRESET_SMALLROOM);

	// SEの再生
	seMana_->PlaySE("Heart");
	// SEの再生
	//seMana_->PlaySE("voice");

}

// 描画
void Enemy::Draw(void)
{
	// モデルの描画
	MV1DrawModel(list.modelInfo_.modelId_);

	//// 視野角の判定
	//DrawFormatString(0, 70, GetColor(255, 255, 255), "視野角 %d", list.isNoticeHearing_);
	//// 聴覚
	//DrawFormatString(0, 90, GetColor(255, 255, 255), "聴覚 %d", list.isNoticeLooking_);
	//// 座標
	//DrawFormatString(0, 110, GetColor(255, 255, 255), "epos = %1.2lf, %1.2lf, %1.2lf", 
	//	list.modelInfo_.pos_.x, list.modelInfo_.pos_.y, list.modelInfo_.pos_.z);

	// デバッグ描画
	//DrawDebug();
}

// 解放
void Enemy::Release(void)
{
	// モデルの解放
	MV1DeleteModel(list.modelInfo_.modelId_);

	// シングルトンの呼び出し
	SoundManager* seMana_ = SoundManager::GetInstance();
	// SEの停止
	seMana_->StopSE("voice");
	seMana_->StopSE("Heart");

	// 解放処理
	seMana_->ReleaseSound("voice");
	seMana_->ReleaseSound("Heart");
}

// 更新
void Enemy::UpdateMove(int stagemodel)
{
	// 自動でエネミー移動
	list.moveVec_ = Application::DEFAULT_VECTOR;

	// シングルトンの呼び出し
	SoundManager* seMana_ = SoundManager::GetInstance();

	// プレイヤーが視覚範囲または聴覚範囲内にいる場合
	if (list.isNoticeLooking_ || list.isNoticeHearing_)
	{
		// 敵に見つかったのでBGMを流す
		seMana_->PlaySE("voice");
		COLOR_F color = GetColorF(1.0f, 0.0f, 0.0f, 1.0f); // 赤色で追跡を示す
		ChasePlayer(stagemodel); // A*アルゴリズムによる追跡処理
	}
	else
	{
		// 敵に見つかっていないのでBGMを消す
		seMana_->StopSE("voice");
		COLOR_F color = GetColorF(0.5f, 0.5f, 0.5f, 1.0f);
		NormalMove();
	}

	// ベクトルの移動が行われていたら座標更新
	if (IsMove(list.moveVec_))
	{
		// 座標更新
		UpdatePos(list.moveVec_);

		list.oldMoveVec_ = list.moveVec_;

		// 方向を角度に変換する( XZ平面 Y軸)
		list.moveVecRad_.y = atan2f(list.moveVec_.x, list.moveVec_.z);

		// シンプルに計算角度を設定してみる
		list.modelInfo_.rot_.y = list.moveVecRad_.y;

		// 座標設定
		MV1SetPosition(list.modelInfo_.modelId_, list.modelInfo_.pos_);
	}
	else
	{
		// 移動状態の設定
	}
}

// 検知
void Enemy::UpdateSearch(void)
{
	// 敵からプレイヤーまでのベクトル
	VECTOR playerFromEnemyVec = VSub(player_->GetInfo().pos_, list.modelInfo_.pos_);

	// プレイヤーと敵の距離を計算する(ピタゴラスの定理より)
	float dis = hypotf(playerFromEnemyVec.x, playerFromEnemyVec.z);

	if (dis <= HEARING_RANGE)
	{
		// 聴覚範囲内であるというフラグを立てる
		list.isNoticeHearing_ = true;

		// 自分から見たプレイヤーの角度を求める
		float rad = atan2(playerFromEnemyVec.x, playerFromEnemyVec.z);

		// 角度の差を求める
		float diffViewRad = rad - list.modelInfo_.rot_.y;

		// ラジアンをデグリーに変換(30度といったデグリーの値のほうが感覚としてわかりやすいため)
		float diffViewDeg = diffViewRad * Application::RAD_TO_DEG;

		// 差の大きさを出す
		diffViewDeg = abs(diffViewDeg);

		// 角度差が視野範囲内だったら視界内判定を行う
		if (diffViewDeg <= VIEW_ANGLE)
		{
			list.isNoticeLooking_ = true;
		}
		else
		{
			list.isNoticeLooking_ = false;
		}
	}
	else
	{
		// 聴覚範囲外であるというフラグを立てる
		list.isNoticeHearing_ = false;

		// 聴覚範囲に入っていなければ視界内に入っていないとする
		list.isNoticeLooking_ = false;
	}
}

void Enemy::UpdatePos(VECTOR vec)
{
	list.moveVec_ = VNorm(vec); // 正規化した移動ベクトル
	list.moveVec_ = VScale(list.moveVec_, list.moveSpeed_); // 速度を適用
	list.modelInfo_.pos_ = VAdd(list.modelInfo_.pos_, list.moveVec_); // 座標を更新
}

// デバッグ描画
void Enemy::DrawDebug(void)
{
	// モデルの移動方向
	if (IsMove(list.moveVec_))
	{
		// 移動ベクトルを正規化
		VECTOR debugMoveVec = VNorm(list.moveVec_);

		// 線の長さを設定
		constexpr float DEBUG_MOVE_LINE_LENGTH = 100.0f;
		debugMoveVec = VScale(debugMoveVec, DEBUG_MOVE_LINE_LENGTH);

		// 線の終端座標を設定
		VECTOR debugMoveVecLineEndPos = VAdd(list.modelInfo_.pos_, debugMoveVec);

		// 移動方向に線を描画
		DrawLine3D(list.modelInfo_.pos_, debugMoveVecLineEndPos, 0xffffff);

		// 敵の位置に当たり判定用の球を描画
		DrawSphere3D(list.modelInfo_.pos_, 200.0f, 20, 0x0000ff, 0x0000ff, false);
		// 敵が InMapMap 上でどこにいるのかを表示
		DrawFormatString(20, 740, GetColor(255, 255, 255), "敵のマップ座標 : [%d][%d}", list.zSet, list.xSet);
		// 視野角の判定
		DrawFormatString(20, 700, GetColor(255, 255, 255), "視野角 %d", list.isNoticeHearing_);
		// 聴覚
		DrawFormatString(20, 800, GetColor(255, 255, 255), "聴覚 %d", list.isNoticeLooking_);

		// 視野角の範囲を円で描画
		DrawSphere3D(list.modelInfo_.pos_, VIEW_ANGLE, 32, 0x0000ff, 0x0000ff, false);

		// 聴覚範囲の範囲を円で描画
		DrawSphere3D(list.modelInfo_.pos_, HEARING_RANGE, 32, 0x0000ff, 0x0000ff, false);
	}

	// 敵が InMapMap 上でどこにいるのかを表示
	DrawFormatString(20, 740, GetColor(255, 255, 255), "敵のマップ座標 : [%d][%d}", list.zSet, list.xSet);
}

// ベクトルの移動判定
bool Enemy::IsMove(VECTOR _moveVec)
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
	if (isNoMoveX && isNoMoveY && isNoMoveZ)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// 通常の移動に戻る
void Enemy::NormalMove()
{
	switch (list.moveDir_)
	{
	case 0:	// 前
		list.moveVec_ = VAdd(list.moveVec_, FRONT_MOVE_VEC);
		ProcessFront();
		break;
	case 1: // 後
		list.moveVec_ = VAdd(list.moveVec_, BACK_MOVE_VEC);
		ProcessBack();
		break;
	case 2: // 右
		list.moveVec_ = VAdd(list.moveVec_, RIGHT_MOVE_VEC);
		ProcessRight();
		break;
	case 3: // 左
		list.moveVec_ = VAdd(list.moveVec_, LEFT_MOVE_VEC);
		ProcessLeft();
		break;
	}
}


// X座標がマップ内でどこにいるかの計算
void Enemy::InMapMapX(void)
{
	list.posX = static_cast<int>(list.modelInfo_.pos_.x);	// 座標をint型にする
	list.xNum = list.posX / 600;					// ワールド座標をマップ座標にする
	list.xRest = list.posX % 600;					// マップ座標内での座標(InMapPosと名付けておく)用に余りを計算しておく
	list.xSet = list.xNum * 3;					// マップ座標をさらに分割し、マップ座標内でのマップ座標(InMapMap)を作る

	// InMapPos を InMapMap に変換する
	if (list.xRest <= 50)			// InMapPos が 50 以下
	{
		list.xSet += 0;
	}
	else if (list.xRest < 550)		// InMapPos が 50 以上かつ 550 未満
	{
		list.xSet += 1;
	}
	else						// InMapPos が 550 以上
	{
		list.xSet += 2;
	}
}

// Z座標がマップ内でどこにいるかの計算
void Enemy::InMapMapZ(void)
{
	list.posZ = static_cast<int>(list.modelInfo_.pos_.z);	// 座標をint型にする
	list.zNum = list.posZ / 600;					// ワールド座標をマップ座標にする
	list.zRest = list.posZ % 600;					// マップ座標内での座標(InMapPosと名付けておく)用に余りを計算しておく
	list.zSet = list.zNum * 3;					// マップ座標をさらに分割し、マップ座標内でのマップ座標(InMapMap)を作る

	// InMapPos を InMapMap に変換する
	if (list.zRest <= 50)			// InMapPos が 50 以下
	{
		list.zSet += 0;
	}
	else if (list.zRest < 550)		// InMapPos が 50 以上かつ 550 未満
	{
		list.zSet += 1;
	}
	else						// InMapPos が 550 以上
	{
		list.zSet += 2;
	}
}

// 前に移動する時の処理
void Enemy::ProcessFront(void)
{
	// 前が壁の時
	if (eMap[list.zSet + 1][list.xSet] == 1)
	{
		if (list.modelInfo_.pos_.z >= (list.zNum * 600 + 300))
		{
			list.modelInfo_.pos_.z = (float)(list.zNum * 600 + 300);
			MV1SetPosition(list.modelInfo_.modelId_, list.modelInfo_.pos_);

			if (eMap[list.zSet][list.xSet + 1] == 0 && eMap[list.zSet][list.xSet - 1] == 0)
			{
				// 左右に移動できる時
				list.moveDir_ = GetRand(1) + 2;
			}
			else if (eMap[list.zSet][list.xSet + 1] == 0)
			{
				// 右に移動できるとき
				list.moveDir_ = 2;	// 右に移動する
			}
			else if (eMap[list.zSet][list.xSet - 1] == 0)
			{
				// 左に移動できる時
				list.moveDir_ = 3;	// 左に移動する
			}
			else
			{
				// 行き止まりで、後ろにしか下がれないとき
				list.moveDir_ = 1;	// 後ろに戻る
			}
		}
	}
}

// 後に移動する時の処理
void Enemy::ProcessBack(void)
{
	// 前が壁の時
	if (eMap[list.zSet - 1][list.xSet] == 1)
	{
		if (list.modelInfo_.pos_.z <= (list.zNum * 600 + 300))
		{
			list.modelInfo_.pos_.z = (float)(list.zNum * 600 + 300);
			MV1SetPosition(list.modelInfo_.modelId_, list.modelInfo_.pos_);

			if (eMap[list.zSet][list.xSet + 1] == 0 && eMap[list.zSet][list.xSet - 1] == 0)
			{
				// 左右に移動できる時
				list.moveDir_ = GetRand(1) + 2;
			}
			else if (eMap[list.zSet][list.xSet - 1] == 0)
			{
				// 右に移動できるとき
				list.moveDir_ = 3;	// 右に移動する
			}
			else if (eMap[list.zSet][list.xSet + 1] == 0)
			{
				// 左に移動できる時
				list.moveDir_ = 2;	// 左に移動する
			}
			else
			{
				// 行き止まりで、後ろにしか下がれないとき
				list.moveDir_ = 0;	// 後ろに戻る
			}
		}
	}
}

// 右に移動する時の処理
void Enemy::ProcessRight(void)
{
	// 前が壁の時
	if (eMap[list.zSet][list.xSet + 1] == 1)
	{
		// 升目の中央に来た時
		if (list.modelInfo_.pos_.x >= (list.xNum * 600 + 300))
		{
			list.modelInfo_.pos_.x = (float)(list.xNum * 600 + 300);
			MV1SetPosition(list.modelInfo_.modelId_, list.modelInfo_.pos_);

			if (eMap[list.zSet + 1][list.xSet] == 0 && eMap[list.zSet - 1][list.xSet] == 0)
			{
				// 左右に移動できる時
				list.moveDir_ = GetRand(1);
			}
			else if (eMap[list.zSet - 1][list.xSet] == 0)
			{
				// 右に移動できるとき
				list.moveDir_ = 1;	// 右に移動する
			}
			else if (eMap[list.zSet + 1][list.xSet] == 0)
			{
				// 左に移動できる時
				list.moveDir_ = 0;	// 左に移動する
			}
			else
			{
				// 行き止まりで、後ろにしか下がれないとき
				list.moveDir_ = 3;	// 後ろに戻る
			}
		}
	}
}

// 左に移動する時の処理
void Enemy::ProcessLeft(void)
{
	// 前が壁の時
	if (eMap[list.zSet][list.xSet - 1] == 1)
	{
		if (list.modelInfo_.pos_.x <= (list.xNum * 600 + 300))
		{
			list.modelInfo_.pos_.x = (float)(list.xNum * 600 + 300);
			MV1SetPosition(list.modelInfo_.modelId_, list.modelInfo_.pos_);

			if (eMap[list.zSet][list.xSet + 1] == 0 && eMap[list.zSet][list.xSet - 1] == 0)
			{
				// 左右に移動できる時
				list.moveDir_ = GetRand(1);
			}
			else if (eMap[list.zSet + 1][list.xSet] == 0)
			{
				// 右に移動できるとき
				list.moveDir_ = 0;	// 右に移動する
			}
			else if (eMap[list.zSet - 1][list.xSet] == 0)
			{
				// 左に移動できる時
				list.moveDir_ = 1;	// 左に移動する
			}
			else
			{
				// 行き止まりで、後ろにしか下がれないとき
				list.moveDir_ = 2;	// 後ろに戻る
			}
		}
	}
}

void Enemy::IsCheckStan(void)
{
	// 敵を止める時間(仮)
	if (list.stanFlg_)
	{
		SceneManager& ins = SceneManager::GetInstance();
		ins.GetDeltaTime();

		stopTime -= ins.GetDeltaTime();

		// stopTimeが0.0f以下
		if (stopTime < 0.0f)
		{
			SetStanFlg(false);
		}
	}
}

//----------------------
// セッター関数
//----------------------
// 敵の座標を設定する
void Enemy::SetEnemyPos(const VECTOR& pos)
{
	list.modelInfo_.pos_ = pos;
	MV1SetPosition(list.modelInfo_.modelId_, list.modelInfo_.pos_);
}

// 回転設定
void Enemy::UpdateRotation(void)
{
	//------------------------------------
	// 回転行列を使用した角度設定
	//------------------------------------

	// 単位行列を設定する
	MATRIX mat = MGetIdent();

	// モデル自体のY軸回転行列を作成する
	MATRIX mGetRotY = MGetRotY(list.modelInfo_.rot_.y);

	// モデルの補正用Y軸回転行列を作成する
	MATRIX mGetLocalRotY = MGetRotY(list.localRot_.y);

	// 行列を合成
	mat = MMult(mat, mGetRotY);
	mat = MMult(mat, mGetLocalRotY);

	// 行列を使用してモデルの角度を設定
	MV1SetRotationMatrix(list.modelInfo_.modelId_, mat);
}

// 速度設定
void Enemy::SetEnemySpeed(const VECTOR& speed)
{
	list.moveVec_ = speed;
}

// アニメーション更新
void Enemy::UpdateAnimation(void)
{
	// 新しいアニメーション再生時間をセット
	list.currentAnimTime_ += ANIM_SPEED;

	if (list.currentAnimTime_ > list.animTotalTime_)
	{
		list.currentAnimTime_ = 0.0f;
	}

	// モデルに指定時間のアニメーションを設定する
	MV1SetAttachAnimTime(list.modelInfo_.modelId_, list.animAttachNo_, list.currentAnimTime_);
}


//-----------------------
// 経路探索関連
//-----------------------

// ヒューリスティック関数（マンハッタン距離）
int Enemy::heuristic(int x1, int z1, int x2, int z2)
{
	return abs(x1 - x2) + abs(z1 - z2);
}

// 敵がプレイヤーを追跡する処理
void Enemy::ChasePlayer(int stagemodel)
{
	int startX = list.xNum + list.xSet;
	int startZ = list.zNum + list.zSet;
	int endX = player_->GetInfo().xNum + player_->GetInfo().xSet;
	int endZ = player_->GetInfo().zNum + player_->GetInfo().zSet;


	// A*アルゴリズムで経路を計算
	// プレイヤーのいる升目が変わるたびに経路探索しているから処理が重い？
	path = AStar(startX, startZ, endX, endZ, eMap);

	// プレイヤーとのマンハッタン距離を計算
	int distance = heuristic(endX, endZ, startX, startZ);

	// プレイヤーと同じマス目にいるときは、追従処理にする
	// もしくは、プレイヤーまでレイを飛ばし、ステージと当たらなかったら、追従処理にする
	VECTOR pPos = player_->GetInfo().cap.end;
	auto info = MV1CollCheck_Line(
		stagemodel,
		-1,
		list.cap.end,
		pPos
	);

	if (info.HitFlag == false || distance < 1)
	{
		// プレイヤーの位置に向かうベクトルを計算
		VECTOR targetVec = VSub(player_->GetInfo().pos_, list.modelInfo_.pos_);

		UpdatePos(targetVec);
	}
	else if (!path.empty())	// 追いかける経路が有効か確認					
	{
		// プレイヤーの位置に到達していない場合に次の移動先を設定
		if (startX != endX || startZ != endZ)
		{
			// path の要素の個数が
			// ２個以上なら１を
			// １個以下なら０を返す
			(path.size() > 1) ? NextPos(1) : NextPos(0);
		}
	}
	else
	{
		// 経路が見つからない場合の対処（例えば、停止や別の動作）
		NormalMove();
	}
}

// 次の移動先を決定
void Enemy::NextPos(int index)
{
	// 次の移動先を決定（マップ座標をワールド座標に変換）
	VECTOR nextPos = 
	{
		(float)(path[index].first * 600 + 300),
		0.0f,
		(float)(path[index].second * 600 + 300)
	};

	// 移動方向ベクトルを計算
	VECTOR targetVec = VSub(nextPos, list.modelInfo_.pos_);

	UpdatePos(targetVec);

	// 目標位置に近い場合は次の経路ノードに切り替え
	if (abs(targetVec.x) < 10.0f && abs(targetVec.z) < 10.0f)
	{
		// 次のノードを現在位置とする
		list.xNum = path[index].first; 
		list.zNum = path[index].second;
	}
}

// A*アルゴリズムによる経路探索関数
// startX : 始点のX座標、 startZ : 始点のZ座標
// endX	  : 終点のX座標、 endZ   : 終点のZ座標
// eMap   : マップの情報(2次元配列、0は通行可能、1は壁とする)
// 最短経路座標(ペアのリスト)。経路が見つからない場合は空のリストを返す
std::vector<std::pair<int, int>> Enemy::AStar(int startX, int startZ, int endX, int endZ, int eMap[EMAP_NUM_Z][EMAP_NUM_X])
{
	//--------------------------------------------------------------------------------------------------------------
	// 配列の定義と初期化
	//--------------------------------------------------------------------------------------------------------------
	
	// 探索予定のノードを保持する優先度付きキュー
	// std::priority_queue<T, Container, Compare>
	// T		 : 要素の型(Node)
	// Container : 内部実装のコンテナクラス(std::vector<>)
	// Compare   : 優先順に並び変えるための比較用述語型(CompareNode)
	// 動的な二次元のキュー配列
	// std::shared_ptr : 同一のメモリの所有権を複数で共有できるようにしたスマートポインタ
	std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNode> openList;

	// 探索済みのノードを管理
	// bool型の動的二次元配列
	std::vector<std::vector<bool>> closedList(EMAP_NUM_Z, std::vector<bool>(EMAP_NUM_X, false));

	// ノードの管理配列
	// 所有権を複数で共有したNode型の動的二次元配列
	//std::vector<std::vector<std::shared_ptr<Node>>> nodes(EMAP_NUM_Z, std::vector<std::shared_ptr<Node>>(EMAP_NUM_X, nullptr));

	// 開始ノードを作成しオープンリストに追加
	auto startNode = std::make_shared<Node>(startX, startZ, 0, heuristic(startX, startZ, endX, endZ));

	//--------------------------------------------------------------------------------------------------------------


	// 開始ノードをオープンリストに追加
	openList.push(startNode);


	//--------------------------------------------------------------------------------------------------------------
	// A*アルゴリズムの探索ループ
	//--------------------------------------------------------------------------------------------------------------
	while (!openList.empty())
	{
		// オープンリストから総コストが最小のノードを取得
		auto currentNode = openList.top();

		// 取得したノードをオープンリストから削除
		openList.pop();

		int currentX = currentNode->xNum;
		int currentZ = currentNode->zNum;

		// 無効な座標はスキップ
		if (currentNode->xNum < 0 || currentNode->xNum >= EMAP_NUM_X ||
			currentNode->zNum < 0 || currentNode->zNum >= EMAP_NUM_Z)
			continue;

		// 目的地に到達した場合、経路を再構築して返す
		if (currentX == endX && currentZ == endZ)
		{
			while (currentNode)
			{
				// pathの中身は終点から始点の順になっているので
				// 後にreverseで逆順に入れ替える必要がある
				path.emplace_back(currentX, currentZ); // 経路を構築
				currentNode = currentNode->parent;	   // 親ノードに遡る
			}

			// 経路を始点から順に並び替え
			std::reverse(path.begin(), path.end());

			// 始点から終点までの道のりを返す
			return path;
		}


		// すでに探索済みならスキップ
		if (closedList[currentNode->zNum][currentNode->xNum]) continue;

		// 現在のノードを探索済みにする
		closedList[currentZ][currentX] = true;


		// 隣接ノードを探索
		//（上下左右の4方向）			  上	　右　　　 左　　  下
		const int directions[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} }; // 上下左右の移動方向		

		for (auto& dir : directions)
		{
			int nextX = currentX + dir[0];
			int nextZ = currentZ + dir[1];

			// マップ外、壁、または探索済みの場合はスキップ
			if (nextX < 0 || nextZ < 0 || nextX >= EMAP_NUM_X || nextZ >= EMAP_NUM_Z ||		// マップ外
				eMap[nextZ][nextX] == 1 ||													// 壁
				closedList[nextZ][nextX])													// 探索済み
				continue;

			// 新しいノードのコスト計算
			int newStartCost = currentNode->startCost + 1;

			// 隣接ノードを作成
			auto neighborNode = std::make_shared<Node>
				(nextX, nextZ, newStartCost, heuristic(nextX, nextZ, endX, endZ), currentNode);

			// オープンリストに追加
			openList.push(neighborNode);
		}
	}
	// 探索ループ終了-----------------------------------------------------------------------------------------------


	return {}; // 経路が見つからない場合は空のリストを返す
}