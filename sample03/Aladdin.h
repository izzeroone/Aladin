#pragma once
/*
update 22/11/2015
by Luu The Vinh

- Cập nhật xét ca chạm với Land, Water
- Chuyển động nhân vật

update 30/11/2015
- Hiệu ứng nhảy xuông nước
*/

#ifndef __ALADDIN_H__
#define __ALADDIN_H__

#include "define.h"
#include "Animation.h"
#include "InputController.h"
#include "BaseObject.h"


#define ALADDIN_MOVE_SPEED 125
#define ALADDIN_JUMP_VEL -450
#define GRAVITY 801
#define PROTECT_TIME 3000
#define GROUND 400

[event_receiver(native)]
class Aladdin : public BaseObject, public IControlable
{
public:
	Aladdin(int life = 3);
	~Aladdin();

	void init();
	void updateInput(float dt);
	void update(float deltatime);
	void draw(LPD3DXSPRITE spriteHandle);
	void release();

	void onKeyPressed(KeyEventArg* key_event);
	void onKeyReleased(KeyEventArg* key_event);


	void setStatus(eStatus status) override;

	void removeGravity();
	void forceMoveRight();
	void unforceMoveRight();
	void forceMoveLeft();
	void unforceMoveLeft();
	void forceJump();
	void unforceJump();
	void unhookinputevent();
	// Character action.
	void standing();
	void moveLeft();
	void moveRight();
	void jump();

	void falling();

	float getMovingSpeed();

	RECT getBounding() override;

private:
	map<int, Animation*> _animations;
	map<string, IComponent*> _componentList;

	float _movingSpeed;

	bool _canJumpDown;
	eStatus _currentAnimateIndex;



	float _protectTime;



	GVector2 getVelocity();

	void updateStatus(float dt);

	void updateCurrentAnimateIndex();

	eDirection getAimingDirection();


	// reset các thuộc tính lại giá trị ban đầu.
	void resetValues();

	BaseObject* _preObject;
	bool _isHolding;
};

void safeCheckCollision(BaseObject* activeobj, BaseObject* passiveobj, float dt);


#endif // !__Aladdin_H__


