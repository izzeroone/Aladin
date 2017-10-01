#include "Bill.h"

Bill::Bill(int life) : BaseObject(eID::BILL)
{
	_canJumpDown = true;
}

Bill::~Bill()
{

}

void Bill::init()
{
	__hook(&InputController::__eventkeyPressed, _input, &Bill::onKeyPressed);
	__hook(&InputController::__eventkeyReleased, _input, &Bill::onKeyReleased);

	_sprite = SpriteManager::getInstance()->getSprite(eID::BILL);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "normal_01"));
	_sprite->setZIndex(1.0f);
	auto movement = new Movement(GVector2(0, 0), GVector2(0, 0), _sprite);
	_componentList["Movement"] = movement;
	_componentList["Gravity"] = new Gravity(GVector2(0, -GRAVITY), movement);


	_animations[eStatus::NORMAL] = new Animation(_sprite, 0.1f);
	_animations[eStatus::NORMAL]->addFrameRect(eID::BILL, "normal_01", NULL);

	_animations[eStatus::NORMAL | eStatus::SHOOTING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::NORMAL | eStatus::SHOOTING]->addFrameRect(eID::BILL, "normal_01", "normal_02", NULL);

	_animations[eStatus::RUNNING] = new Animation(_sprite, 0.12f);
	_animations[eStatus::RUNNING]->addFrameRect(eID::BILL, "run_01", "run_02", "run_03", "run_04", "run_05", "run_06", NULL);

	_animations[eStatus::JUMPING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::JUMPING]->addFrameRect(eID::BILL, "jump_01", "jump_02", "jump_03", NULL);

	_animations[eStatus::FALLING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::FALLING]->addFrameRect(eID::BILL, "run_03", NULL);

	_animations[eStatus::LAYING_DOWN] = new Animation(_sprite, 0.1f);
	_animations[eStatus::LAYING_DOWN]->addFrameRect(eID::BILL, "lay_down_01", NULL);

	/*_animations[eStatus::LOOKING_UP] = new Animation(_sprite, 0.1f);
	_animations[eStatus::LOOKING_UP]->addFrameRect(eID::BILL, "shot_up_01", NULL);

	_animations[eStatus::LOOKING_UP | eStatus::SHOOTING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::LOOKING_UP | eStatus::SHOOTING]->addFrameRect(eID::BILL, "shot_up_01", "shot_up_02", NULL);

	_animations[eStatus::LOOKING_UP | eStatus::RUNNING] = new Animation(_sprite, 0.12f);
	_animations[eStatus::LOOKING_UP | eStatus::RUNNING]->addFrameRect(eID::BILL, "run_shot_up_01", "run_shot_up_02", "run_shot_up_03", NULL);

	_animations[eStatus::LAYING_DOWN | eStatus::RUNNING] = new Animation(_sprite, 0.12f);
	_animations[eStatus::LAYING_DOWN | eStatus::RUNNING]->addFrameRect(eID::BILL, "run_shot_down_01", "run_shot_down_02", "run_shot_down_03", NULL);

	_animations[eStatus::SHOOTING | eStatus::RUNNING] = new Animation(_sprite, 0.12f);
	_animations[eStatus::SHOOTING | eStatus::RUNNING]->addFrameRect(eID::BILL, "run_shot_01", "run_shot_02", "run_shot_03", "run_shot_01", "run_shot_02", "run_shot_03", NULL);

	_animations[eStatus::SWIMING] = new Animation(_sprite, 0.2f, false);
	_animations[eStatus::SWIMING]->addFrameRect(eID::BILL, "swim_begin", "diving", "swimming", "swimming_01", NULL);
	_animations[eStatus::SWIMING]->animateFromTo(0, 2, false);

	_animations[eStatus::DIVING] = new Animation(_sprite, 0.2f, false);
	_animations[eStatus::DIVING]->addFrameRect(eID::BILL, "swim_begin", "diving", "diving_01", NULL);
	_animations[eStatus::DIVING]->animateFromTo(0, 1, false);

	_animations[eStatus::SWIMING | RUNNING | SHOOTING] = new Animation(_sprite, 0.2f, false);
	_animations[eStatus::SWIMING | RUNNING | SHOOTING]->addFrameRect(eID::BILL, "swim_begin", "swimming_shot", "swimming_shot_01", NULL);
	_animations[eStatus::SWIMING | RUNNING | SHOOTING]->animateFromTo(0, 1, false);

	_animations[eStatus::SWIMING | SHOOTING] = new Animation(_sprite, 0.2f, false);
	_animations[eStatus::SWIMING | SHOOTING]->addFrameRect(eID::BILL, "swim_begin", "swimming_shot", "swimming_shot_01", NULL);
	_animations[eStatus::SWIMING | SHOOTING]->animateFromTo(0, 1, false);

	_animations[eStatus::SWIMING | RUNNING | LOOKING_UP | SHOOTING] = new Animation(_sprite, 0.2f, false);
	_animations[eStatus::SWIMING | RUNNING | LOOKING_UP | SHOOTING]->addFrameRect(eID::BILL, "swim_begin", "swimming_shotup", "swimming_shotup_01", NULL);
	_animations[eStatus::SWIMING | RUNNING | LOOKING_UP | SHOOTING]->animateFromTo(0, 1, false);

	_animations[eStatus::SWIMING | LOOKING_UP | SHOOTING] = new Animation(_sprite, 0.2f, false);
	_animations[eStatus::SWIMING | LOOKING_UP | SHOOTING]->addFrameRect(eID::BILL, "swim_begin", "swimming_shotup_stand", "swimming_shotup_stand_01", NULL);
	_animations[eStatus::SWIMING | LOOKING_UP | SHOOTING]->animateFromTo(0, 1, false);

	_animations[eStatus::DYING] = new Animation(_sprite, 0.2f, false);
	_animations[eStatus::DYING]->addFrameRect(eID::BILL, "dead_01", "dead_02", "dead_03", "dead_04", NULL);*/

	this->setOrigin(GVector2(0.5f, 0.0f));
	this->setStatus(eStatus::FALLING);


	_isHolding = false;

	this->resetValues();

}

void Bill::update(float deltatime)
{
	// đang protect thì đếm xuống
	if (_protectTime > 0)
	{
		_protectTime -= deltatime;
	}


	this->updateStatus(deltatime);

	this->updateCurrentAnimateIndex();

	_animations[_currentAnimateIndex]->update(deltatime);
	// update ground

	if (this->getPositionY() > GROUND)
	{
		this->setPositionY(GROUND);
		auto gravity = (Gravity*)this->_componentList["Gravity"];
		gravity->setStatus(eGravityStatus::SHALLOWED);
		this->standing();
	}

	// update component để sau cùng để sửa bên trên sau đó nó cập nhật đúng
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}

}


/*
*	author: Tung Ho
*	datemodified : 27/11/2015
*	kiểm tra và xoá đạn..
*	xoá các đạn ra khỏi màn hình và các đạn có status = destroy
*/


void Bill::resetValues()
{
	this->setScale(SCALE_FACTOR);


	_preObject = nullptr;


	_protectTime = PROTECT_TIME;
	_movingSpeed = BILL_MOVE_SPEED;


	for (auto animate : _animations)
	{
		animate.second->setColorFlash(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void Bill::updateInput(float dt)
{
	if (this->isInStatus(eStatus::DYING))
		return;

}

void Bill::draw(LPD3DXSPRITE spriteHandle)
{
	if (_protectTime > 0)
	{
		_animations[_currentAnimateIndex]->enableFlashes(true);

	}
	else
	{
		_animations[_currentAnimateIndex]->enableFlashes(false);
	}


	_animations[_currentAnimateIndex]->draw(spriteHandle);


}

void Bill::release()
{
	for (auto it = _animations.begin(); it != _animations.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_animations.clear();

	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_componentList.clear();

	SAFE_DELETE(_sprite);

	this->unhookinputevent();
}

void Bill::onKeyPressed(KeyEventArg* key_event)
{
	if (this->isInStatus(eStatus::DYING))
		return;

	switch (key_event->_key)
	{
	case DIK_X:
	{
		if (!this->isInStatus(eStatus::LAYING_DOWN) || this->isInStatus(eStatus::MOVING_LEFT) || this->isInStatus(eStatus::MOVING_RIGHT))
		{
			if (!this->isInStatus(eStatus::SWIMING))
				this->jump();
		}
		else
		{
			if (_canJumpDown && !this->isInStatus(eStatus::JUMPING) && !this->isInStatus(eStatus::FALLING))
			{
				this->removeStatus(eStatus::SHOOTING);
				this->addStatus(eStatus::JUMPING);
				this->addStatus(eStatus::FALLING);
			}

		}

		break;
	}
	case DIK_LEFT:
	{
		if (this->isInStatus(eStatus::DIVING))
			return;

		this->removeStatus(eStatus::LAYING_DOWN);
		this->removeStatus(eStatus::MOVING_RIGHT);
		this->addStatus(eStatus::MOVING_LEFT);

		break;
	}
	case DIK_RIGHT:
	{
		if (this->isInStatus(eStatus::DIVING))
			return;

		this->removeStatus(eStatus::LAYING_DOWN);
		this->removeStatus(eStatus::MOVING_LEFT);
		this->addStatus(eStatus::MOVING_RIGHT);

		break;
	}
	case DIK_DOWN:
	{
		if (!this->isInStatus(eStatus::SWIMING))
		{
			this->removeStatus(eStatus::LOOKING_UP);
			this->addStatus(eStatus::LAYING_DOWN);
		}
		else
		{
			this->addStatus(eStatus::DIVING);
			this->removeStatus(eStatus::MOVING_LEFT);
			this->removeStatus(eStatus::MOVING_RIGHT);
		}

		break;
	}
	case DIK_UP:
	{
		// không có TH nằm mà bắn lên
		if (!this->isInStatus(eStatus::LAYING_DOWN))
			this->addStatus(eStatus::LOOKING_UP);

		break;
	}
	case DIK_Z:
	{
		if (this->isInStatus(eStatus::DIVING))
			return;

		_isHolding = true;

		break;
	}
#if _DEBUG
	case DIK_1:
	case DIK_2:
	case DIK_3:
	case DIK_4:
	case DIK_5:
	case DIK_6:
	case DIK_7:
	{
		break;
	}
	case DIK_L:
	{
		break;
	}
#endif
	default:
		break;
	}
}

void Bill::onKeyReleased(KeyEventArg * key_event)
{
	if (this->isInStatus(eStatus::DYING))
		return;

	switch (key_event->_key)
	{
	case DIK_RIGHT:
	{
		this->removeStatus(eStatus::MOVING_RIGHT);
		break;
	}
	case DIK_LEFT:
	{
		this->removeStatus(eStatus::MOVING_LEFT);
		break;
	}
	case DIK_DOWN:
	{
		this->removeStatus(eStatus::LAYING_DOWN);
		this->removeStatus(eStatus::DIVING);
		break;
	}
	case DIK_UP:
	{
		this->removeStatus(eStatus::LOOKING_UP);
		break;
	}
	case DIK_Z:
	{
		//if ((_currentGun & eBulletType::R_BULLET) == eBulletType::R_BULLET)
		_isHolding = false;

		//	this->removeStatus(eStatus::SHOOTING);

		break;
	}
	default:
		break;
	}
}



void Bill::standing()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, 0));

	this->removeStatus(eStatus::JUMPING);
	this->removeStatus(eStatus::FALLING);
}

void Bill::moveLeft()
{
	float billPositionX = this->getPositionX();
	auto halfwidth = this->getSprite()->getFrameWidth() * this->getSprite()->getOrigin().x;
	// Không cho đi vượt cạnh trái
	if (billPositionX + halfwidth - _movingSpeed * 0.33 <= 0) // hard code
	{
		this->setPositionX(0+ halfwidth);
		return;
	}
	if (this->getScale().x > 0)
		this->setScaleX(this->getScale().x * (-1));

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(-_movingSpeed, move->getVelocity().y));
}

void Bill::moveRight()
{
	if (this->getScale().x < 0)
		this->setScaleX(this->getScale().x * (-1));

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(_movingSpeed, move->getVelocity().y));
}

void Bill::jump()
{
	if (this->isInStatus(eStatus::JUMPING) || this->isInStatus(eStatus::FALLING))
		return;

	this->addStatus(eStatus::JUMPING);

	//if (!this->isInStatus(eStatus::LAYING_DOWN))
	{
		auto move = (Movement*)this->_componentList["Movement"];
		move->setVelocity(GVector2(move->getVelocity().x, BILL_JUMP_VEL));
	}

	auto g = (Gravity*)this->_componentList["Gravity"];
	g->setStatus(eGravityStatus::FALLING__DOWN);

}
void Bill::falling()
{
	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::FALLING__DOWN);
}
void Bill::setStatus(eStatus status)
{
	// lặn / đang được bảo vệ  ko có chết
	if ((status & eStatus::DYING) == eStatus::DYING && (this->isInStatus(eStatus::DIVING) || _protectTime > 0))
		return;

	_status = status;
}

RECT Bill::getBounding()
{
	int offset = 10;

	RECT bound = _sprite->getBounding();

	if (!this->isInStatus(eStatus::JUMPING) || !this->isInStatus(eStatus::SWIMING))
	{
		if ((_currentAnimateIndex & LOOKING_UP) == LOOKING_UP)
		{
			bound.top -= offset * 2.0f;
		}
		else
		{
			bound.top -= offset;
		}

		if (this->getScale().x > 0)
		{
			bound.right -= offset;
			bound.left += offset * 0.2f;
		}
		else
		{
			bound.left += offset;
			bound.right -= offset * 0.2f;
		}
	}

	return bound;
}

GVector2 Bill::getVelocity()
{
	auto move = (Movement*)this->_componentList["Movement"];
	return move->getVelocity();
}

void Bill::updateStatus(float dt)
{

	if ((this->getStatus() & eStatus::MOVING_LEFT) == eStatus::MOVING_LEFT)
	{
		this->moveLeft();
	}
	else if ((this->getStatus() & eStatus::MOVING_RIGHT) == eStatus::MOVING_RIGHT)
	{
		this->moveRight();
	}
	else if ((this->getStatus() & eStatus::JUMPING) != eStatus::JUMPING)
	{
		this->standing();
	}
}

void Bill::updateCurrentAnimateIndex()
{
	// _currentAnimateIndex là key của animate thôi nên có một số bị dư, nếu dùng chung status
	// vậy cần convert lại tương ứng với key trong animate

	if (this->isInStatus(eStatus::SHOOTING) && !this->isInStatus(eStatus::SWIMING) && (this->isInStatus(eStatus::LOOKING_UP) || this->isInStatus(eStatus::LAYING_DOWN)) && (this->isInStatus(eStatus::MOVING_LEFT) || this->isInStatus(eStatus::MOVING_RIGHT)))
	{
		// đang di chuyển mà shoot
		// animate ko có shooting nên bỏ nó ra
		_currentAnimateIndex = (eStatus)(this->getStatus() & ~(eStatus::SHOOTING));
	}
	else if (this->isInStatus(eStatus::LAYING_DOWN) && this->isInStatus(eStatus::SHOOTING) && !this->isInStatus(eStatus::FALLING))
	{
		// đang nằm và bắn
		// và ko có đang nhảy, thì animate NẰM thôi
		if (!this->isInStatus(eStatus::JUMPING))
			_currentAnimateIndex = eStatus::LAYING_DOWN;
		else
			_currentAnimateIndex = eStatus::JUMPING;
	}
	else
	{
		// trường hợp còn lại gán luôn
		_currentAnimateIndex = this->getStatus();
	}

	if ((_currentAnimateIndex & eStatus::FALLING) == eStatus::FALLING)
	{
		_currentAnimateIndex = eStatus::FALLING;

	}

	if ((_currentAnimateIndex & eStatus::HOLDING) == eStatus::HOLDING)
	{
		// không có animate HOLDING nên bỏ nó ra
		_currentAnimateIndex = (eStatus)(_currentAnimateIndex & ~(eStatus::HOLDING));
	}

	if ((_currentAnimateIndex & eStatus::MOVING_LEFT) == eStatus::MOVING_LEFT || ((_currentAnimateIndex & eStatus::MOVING_RIGHT) == eStatus::MOVING_RIGHT))
	{
		// animate move left/right xài cung 1 animate là running nên cũng bỏ nó ra
		_currentAnimateIndex = (eStatus)(_currentAnimateIndex & ~(eStatus::MOVING_LEFT | MOVING_RIGHT));
		_currentAnimateIndex = (eStatus)(_currentAnimateIndex | eStatus::RUNNING);
	}

	if ((_currentAnimateIndex & eStatus::JUMPING) == eStatus::JUMPING)
	{
		// nếu jump thì chỉ vẽ jump thôi
		_currentAnimateIndex = eStatus::JUMPING;
	}

	// đang bơi
	if ((_currentAnimateIndex & eStatus::SWIMING) == eStatus::SWIMING)
	{
		if ((_currentAnimateIndex & eStatus::SHOOTING) != eStatus::SHOOTING)
		{
			_currentAnimateIndex = eStatus::SWIMING;
		}

		if (this->isInStatus(eStatus::DIVING))
		{
			_currentAnimateIndex = eStatus::DIVING;
		}
	}

	// chết
	if (this->isInStatus(eStatus::DYING))
	{
		_currentAnimateIndex = eStatus::DYING;
	}
}

eDirection Bill::getAimingDirection()
{
	eDirection direction;

	if (this->getScale().x < 0)
		direction = eDirection::LEFT;
	else
		direction = eDirection::RIGHT;

	if (this->isInStatus(eStatus::FALLING))
		return direction;

	if (this->isInStatus(eStatus::LOOKING_UP))
	{
		if (_input->getInstance()->isKeyDown(DIK_LEFT) || _input->getInstance()->isKeyDown(DIK_RIGHT))
			direction = (eDirection)(direction | eDirection::TOP);
		else
			direction = eDirection::TOP;
	}
	else if (this->isInStatus(eStatus::LAYING_DOWN))
	{
		if (_input->getInstance()->isKeyDown(DIK_LEFT) || _input->getInstance()->isKeyDown(DIK_RIGHT))
			direction = (eDirection)(direction | eDirection::BOTTOM);
		else if (this->isInStatus(eStatus::JUMPING))
			direction = eDirection::BOTTOM;
	}

	return direction;
}

void Bill::forceMoveRight()
{
	onKeyPressed(new KeyEventArg(DIK_RIGHT));
}
void Bill::unforceMoveRight()
{
	onKeyReleased(new KeyEventArg(DIK_RIGHT));
}
void Bill::forceMoveLeft()
{
	onKeyPressed(new KeyEventArg(DIK_LEFT));
}
void Bill::unforceMoveLeft()
{
	onKeyReleased(new KeyEventArg(DIK_LEFT));
}
void Bill::forceJump()
{
	onKeyPressed(new KeyEventArg(DIK_X));
}
void Bill::unforceJump()
{
	onKeyReleased(new KeyEventArg(DIK_X));
}
void Bill::removeGravity()
{
	auto graivity = (Gravity*)(this->_componentList.find("Gravity")->second);
	graivity->setGravity(VECTOR2ZERO);
}

void Bill::unhookinputevent()
{
	if (_input != nullptr)
		__unhook(_input);

}

float Bill::getMovingSpeed()
{
	return _movingSpeed;
}

//void safeCheckCollision(BaseObject* activeobj, BaseObject* passiveobj, float dt)
//{
//	if (activeobj != nullptr && passiveobj != nullptr)
//	{
//		activeobj->checkCollision(passiveobj, dt);
//	}
//}
