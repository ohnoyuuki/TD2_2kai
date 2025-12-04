#pragma once
#include"KamataEngine.h"
#include"MyMath.h"
#include"EnemyBullet.h"
#include<list>

class MapChipField;
class Player;
class PlayerBullet;
class Enemy
{
public:


	// デスフラグ
	bool isenemyDead_ = false;
	// デスフラグのgetter
	bool IsEnemyDead() const { return isenemyDead_; }

	/**/
	// 敵の当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	// マップとの当たり判定情報
	struct CollisionMapInfo
	{
		bool ceiling = false;            // 天井衝突フラグ
		bool langing = false;            // 着地フラグ
		bool hitwall = false;            // 壁接触フラグ
		KamataEngine::Vector3 move = {}; // 移動量
	};



	/*
	void CheckMapCollision(CollisionMapInfo& info);
	void CheckMapCollisionUP(CollisionMapInfo& info);    // 上方向
	void CheckMapCollisionDown(CollisionMapInfo& info);  // 下方向
	void CheckMapCollisionRight(CollisionMapInfo& info); // 右方向
	void CheckMapCollisionLeft(CollisionMapInfo& info);  // 左方向
	// 3.判定結果を反映して移動させる
	void CheckMapMove(const CollisionMapInfo& info);
	// 4.天井に接触している場合の処理
	void CheckMapCeiling(const CollisionMapInfo& info);
	// 5.壁に接触している場合の処理
	void CheckMapWall(CollisionMapInfo& info);
	// 6.接地状態の切り替え
	void CheckMapLanding(const CollisionMapInfo& info);
	// 着地フラグ
*/
	

	

	// 歩行の速さ
	static inline const float kWalkSpeed = 0.04f;

	// 最初の角度[度]
	static inline const float kWalkMotionAngleStart = 5.0f;
	
	// 最後の角度[度]
	static inline const float kWalkMotionAngleEnd = 5.0f;
	
	// アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionTime = 5.0f;

	// 経過時間
	float walkTimer_ = 0.0f;






	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();
	// AABBを取得
	AABB GetAABB();
	//衝突応答
	void OnCollition(const Player* player);





	#pragma region 自キャラの弾と敵
	// AABBを取得
	AABB2 GetAABB2();
	// 衝突応答
	void OnCollition2(const PlayerBullet* playerBullet);
	#pragma endregion



	//体力表示
	int enemyHp;



	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position);

	

	// 更新
	void Update();

	// 描画
	void Draw();

	

	

private:


	///////////////
	// 敵左右移動//
	//////////////
	enum class EnemyState
	{
		IdleMove, // 上下に揺れる状態
		MoveLeft, // 左へ移動
		MoveRight // 右へ戻る
	};

	EnemyState state_ = EnemyState::IdleMove;

	// ランダムで行動するためのタイマー
	float actionTimer_ = 0.0f;
	float nextActionTime_ = 0.0f;

	float startX_; // 初期位置X（戻るため）
	//////////////////
	// 敵左右移動終///
	/////////////////



	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// モデル
	KamataEngine::Camera* camera_;
	// テクスチャハンドル
	// uint32_t textureHandle_ = 0u;

	KamataEngine::Model* model_;

	KamataEngine::Vector3 velocity_ = {};


};