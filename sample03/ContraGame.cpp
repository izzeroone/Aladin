#include "ContraGame.h"
#include "spritemanager.h"
#include "Aladdin.h"
ContraGame::ContraGame(HINSTANCE hInstance, LPWSTR title) : Game(hInstance, title, WINDOW_WIDTH, WINDOW_HEIGHT)
{

}

ContraGame::~ContraGame()
{
}

void ContraGame::init()
{
	Game::init();
	// init game
#if _DEBUG
	_bill = new Aladdin(3);
	_bill->init();
	_bill->setPosition(200, 400);

	//Load background
	_Background = CreateSurfaceFromFile(DeviceManager::getInstance()->getDevice(), BACKGROUND1);
	
	//SceneManager::getInstance()->addScene(new Stage3(30));
	//SceneManager::getInstance()->addScene(new PlayScene());
	//SceneManager::getInstance()->addScene(new IntroScene());
#else
	SceneManager::getInstance()->addScene(new IntroScene());
#endif
}

void ContraGame::release()
{
	Game::release();

	// release game
	//SceneManager::getInstance()->clearScenes();
}

void ContraGame::updateInput(float deltatime)
{
	//SceneManager::getInstance()->updateInput(deltatime);
}

void ContraGame::update(float deltatime)
{
	_bill->update(deltatime);
	//SceneManager::getInstance()->update(deltatime);
}

void ContraGame::draw()
{
	DeviceManager::getInstance()->getDevice()->StretchRect(
		_Background,			// from 
		NULL,				// which portion?
		DeviceManager::getInstance()->getSurface(),		// to 
		NULL,				// which portion?
		D3DTEXF_NONE);
	this->_spriteHandle->Begin(D3DXSPRITE_ALPHABLEND);

	_bill->draw(this->_spriteHandle);
//	SceneManager::getInstance()->draw(_spriteHandle);

	this->_spriteHandle->End();

}

void ContraGame::loadResource()
{
	// Game::init đã gọi hàm này rồi nên không cần gọi lại ContraGame::loadResource
	// load resource
	SpriteManager::getInstance()->loadResource(_spriteHandle);
	//SoundManager::getInstance()->loadSound(Game::hWindow->getWnd());

}
