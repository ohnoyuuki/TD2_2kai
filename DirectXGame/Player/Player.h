#pragma once
#include "KamataEngine.h"

/**/

// マップとの当たり判定情報
struct CollisionMapInfo 
{
	// 天井衝突フラグ
	bool celling = false;

	// 着地フラグ
	bool landing = false;

	// 壁接触フラグ
	bool hitWall = false;

	// 移動量
	KamataEngine::Vector3 move;
};


class MapChipField;
class Player 
{
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position);

	void Update();

	void Draw();

	void InputMove();
	void AnimationTurn();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	void CheckMapCollision(CollisionMapInfo& info);
	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapMove(CollisionMapInfo& info);
	void CheckMapCelling(CollisionMapInfo& info);


	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

	

	//キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	//角
	enum Corner
	{
		kRightBottom, //右下
		kLeftBottom,  //左下
		kRightTop,	  //右上
		kLeftTop,	  //左上
	
	    kNumCorner    //要素数
	};
	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);

	static inline const float kBlank = 0;
	

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;
	// KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::Vector3* position_ = nullptr;

	// テクスチャハンドル
	// uint32_t textureHandle_ = 0u;

	KamataEngine::Vector3 velocity_ = {};
	static inline const float kAcceleration = 1.0f;
	static inline const float kAttenuation = 1.0f;
	//プレイヤーの速度
	static inline const float kLimitRunSpeed = 0.3f;

	// 左右
	enum class LRDirection 
	{
		kRight,
		kLeft,
	};
	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;

	bool onGround_ = true;

	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.1f;
	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 1.0f;
	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 1.0f;


	//マップチップフィールド
	MapChipField* mapChipField_ = nullptr;
	
};

