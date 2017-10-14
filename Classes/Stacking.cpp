#include "Stacking.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* Stacking::createScene()
{
    return Stacking::create();
}

bool Stacking::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    initElement();
    initClickListener();
    
    return true;
}

void Stacking::initElement()
{
    
    auto screenSize = Director::getInstance()->getVisibleSize();
    Sprite* firstLayer = Sprite::create("BlueSquare.png");
    
    addChild(firstLayer);
    
    firstLayer->setScale(0.1, 0.02);
    firstLayer->setPosition(Vec2(firstLayer->getBoundingBox().size.width / 2, screenSize.height - firstLayer->getBoundingBox().size.height/2));
    
    layers.push_back(firstLayer);
    runInitialAnimation();
}

void Stacking::runInitialAnimation()
{
    auto screenSize = Director::getInstance()->getVisibleSize();
    auto layer = layers.back();
    
    auto moveRight = MoveTo::create(1, Vec2(screenSize.width - layer->getBoundingBox().size.width/2, layer->getPosition().y));
    auto moveLeft = MoveTo::create(1, Vec2(layer->getBoundingBox().size.width/2, layer->getPosition().y));
    
    auto easeRight = EaseSineInOut::create(moveRight);
    auto easeLeft = EaseSineInOut::create(moveLeft);

	auto changeColor0 = TintTo::create(1, Color3B::BLUE);
	auto changeColor1 = TintTo::create(1, Color3B::MAGENTA);
	auto changeColor2 = TintTo::create(1, Color3B::RED);
	auto changeColor3 = TintTo::create(1, Color3B::ORANGE);
	auto changeColor4 = TintTo::create(1, Color3B::YELLOW);
	auto changeColor5 = TintTo::create(1, Color3B::GREEN);
    
    auto sequence = Sequence::create(easeRight, easeLeft, nullptr);

	auto sequenceColor = Sequence::create(changeColor0, changeColor1, changeColor2, changeColor3, changeColor4, changeColor5, nullptr);
    
    layer->runAction(RepeatForever::create(sequence));
	layer->runAction(RepeatForever::create(sequenceColor));
    
    
}

void Stacking::initClickListener()
{
	auto keyListener = EventListenerKeyboard::create();
    auto mouseListener = EventListenerTouchOneByOne::create();

	//Keyboard down
	keyListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		switch (keyCode) {
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				context->stopAndFall();
				break;
		}
	};
    
    mouseListener->onTouchBegan = [=](Touch* touch, Event* event){
        context->stopAndFall();
        return true;
    };
    mouseListener->onTouchMoved = [=](Touch* touch, Event* event){};
    
    mouseListener->onTouchEnded = [=](Touch* touch, Event* event){};
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Stacking::stopAndFall()
{
    auto layer = layers.back();
    layer->stopAllActions();
    
    if(layers.size() == 1)
    {
        auto funcInitElement = CallFunc::create(CC_CALLBACK_0(Stacking::initElement, this));
        auto fallAnimation = MoveTo::create(0.3f, Vec2(layer->getPosition().x, layer->getBoundingBox().size.height/2));
        
        layer->runAction(Sequence::create(fallAnimation, funcInitElement, nullptr));
    }
    else
    {
        auto funcInitElement = CallFunc::create(CC_CALLBACK_0(Stacking::initElement, this));
        auto fallAnimation = MoveTo::create(0.2f, Vec2(layer->getPosition().x,
                                                       layers.at(layers.size()-2)->getPosition().y + layer->getBoundingBox().size.height));
        
        layer->runAction(Sequence::create(fallAnimation, funcInitElement, nullptr));
    }


}
