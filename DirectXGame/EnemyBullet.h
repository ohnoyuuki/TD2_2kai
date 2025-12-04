#pragma once
#include "KamataEngine.h"
#include "MyMath.h"

class Barrier;
class EnemyBullet 
{
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity);

	void Update();

	void Draw();

	// 終了フラグ
	bool isFinished_ = false;

	// デスフラグ
	bool isDead_ = false;

	bool IsDead() const { return isDead_; }

	// 速度
	KamataEngine::Vector3 velocity_;



	// 当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();
	


	#pragma region 敵弾とバリアの衝突
	
	// AABBを取得
	AABB GetAABB3();
	// 衝突応答
	void OnCollition3(const Barrier* barrier);

	#pragma endregion




private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_;

	// カメラ
	KamataEngine::Camera* camera_;

	// 速度
	KamataEngine::Vector3 Bulletvelocity_;
};
