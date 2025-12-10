#pragma once
#include"KamataEngine.h"
#include"MyMath.h"
#include"EnemyBullet.h"
class MapChipField;

class EnemyBullet;
class Barrier
{
public:
	void Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera, KamataEngine::Vector3& position);

	void Update();

	void Draw();



	
	// デスフラグ
	bool barrierDead_ = false;
	// デスフラグのgetter
	//bool BarrierDead() const { return barrierDead_; }
	// バリアの当たり判定サイズ
	static inline const float kWidth = 30.0f;
	static inline const float kHeight = 720.0f;
	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();


	#pragma region バリアと敵の弾の衝突

	// AABBを取得
	AABB4 GetAABB4();
	// 衝突応答
	void OnCollition4(const EnemyBullet* enemyBullet);

	#pragma endregion



	// 体力表示
	int BarrierHp;
	int B_GetHP() const { return hp_; }
	int B_GetMaxHP() const { return maxHP_; }
	bool BarrierDead() const { return barrierDead_; }


private:
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	int maxHP_ = 50000;
	int hp_ = maxHP_;
};
