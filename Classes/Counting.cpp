//
//  Counting.cpp
//  Sofe-4th-mobile
//
//  Created by Alejos on 10/11/17.
//

#include "Counting.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* Counting::createScene()
{
    return Counting::create();
}

bool Counting::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    initVariables();
    initListeners();
    display();
    return true;
}

void Counting::initVariables()
{
    array = Vector<Sprite*>();
    spawned = 0;
    int objects = RandomHelper::random_int(2,20);
    for(int i = 0; i < objects; i++)
    {
        int object = RandomHelper::random_int(1,3);
        if(object == 1) {
            array.pushBack(Sprite::create("circle.png"));
            circles++;
        }
        if(object == 2) {
            array.pushBack(Sprite::create("square.png"));
            circles++;
        }
        if(object == 3) {
            array.pushBack(Sprite::create("triforce.png"));
            circles++;
        }
    }
}

void Counting::initListeners()
{
	auto sae = CocosDenshion::SimpleAudioEngine::getInstance();
	sae->preloadBackgroundMusic("song.mp3");
    auto despawnListener = EventListenerCustom::create("custom_event_sprite_disappear", [=](EventCustom* event)
    {
        spawned++;
        if (spawned == array.size())
        {
            displayCounter();
        }
    });
    _eventDispatcher->addEventListenerWithFixedPriority(despawnListener, 1);
    
    auto tapListener = EventListenerTouchOneByOne::create();
    tapListener->onTouchBegan = [ this, sae ](Touch* touch, Event* event)
    {
		sae->playBackgroundMusic("sound/point.wav");
        if (plus->getBoundingBox().containsPoint(touch->getLocation())) { displayUp(); }
        if (minus->getBoundingBox().containsPoint(touch->getLocation())){ displayDown(); }
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(tapListener, this);
    
    auto endListener = EventListenerCustom::create("custom_event_end", [=](EventCustom* event)
    {
		sae->end();
       endGame();
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(endListener, this);
}

void Counting::display()
{
    float delay = 0.8;
    for( int i = 0; i < array.size(); i++ )
    {
        auto delayT = DelayTime::create(delay * i);
        auto spriteSpawn = CallFunc::create([ this, i ]()
         {
             Sprite* aux = Sprite::createWithTexture(this->array.at(i)->getTexture());
             aux->setScale(this->array.at(i)->getScaleX(), this->array.at(i)->getScaleY());
             
             aux->setPosition(Vec2(Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height/2));
             
             auto moveToLeft = MoveTo::create(1.5f, Vec2(Director::getInstance()->getVisibleSize().height / 2 - spawned / 10, 0));
             auto removeSprite = RemoveSelf::create();
             auto callListener = CallFunc::create([this]()
              {
                  EventCustom event("custom_event_sprite_disappear");
                  _eventDispatcher->dispatchEvent(&event);
              });
             
             auto sequence = Sequence::create(moveToLeft, removeSprite, callListener, nullptr);
             aux->runAction(sequence);
             
             addChild(aux);
         });
        
        auto seq = Sequence::create(delayT, spriteSpawn, nullptr);
        this->runAction(seq);
    }
}

void Counting::displayCounter()
{
    auto xy = Director::getInstance()->getVisibleSize();
    
    auto myLabel = Label::create();
    myLabel->setString("How many did you find?");
    myLabel->setTextColor(Color4B::WHITE);
    myLabel->setPosition(Vec2(xy.width/2, xy.height/3));
    addChild(myLabel);
    
    counter = 0;
    label = Label::create();
    label->setString("0.0");
    label->setTextColor(Color4B::WHITE);
    label->setPosition(Vec2(xy.width/2, xy.height/4));
    addChild(label);
    
    minus = Sprite::create("minus.png");
    minus->setPosition(Vec2(xy.width/2.5, xy.height/4));
    addChild(minus);
    
    plus = Sprite::create("plus.png");
    plus->setScale(.5);
    plus->setPosition(Vec2(xy.width/1.5 - plus->getBoundingBox().size.width, xy.height/4));
    addChild(plus);
    
    int object = RandomHelper::random_int(1,3);
    Sprite* question;
    switch(object)
    {
        case 1 :
            question = Sprite::create("circle.png");
            answer = circles;
            break;
        case 2 :
            question = Sprite::create("square.png");
            answer = squares;
            break;
        case 3 :
            question = Sprite::create("triangle.png");
            answer = triangles;
            break;
    }
    
    question->setScale(1.0f);
    question->setPosition(Vec2(xy.width/2, xy.height/2));
    
    addChild(question);
    
    int time = 5;
    auto delay = DelayTime::create(time);
    auto end = CallFunc::create([ this ]()
    {
        EventCustom event("custom_event_end");
        _eventDispatcher->dispatchEvent(&event);
    });
    
    auto sequence = Sequence::create(delay, end, nullptr);
    this->runAction(sequence);
}

void Counting::displayUp()
{
    counter++;
    label->setString(std::to_string(counter));
}

void Counting::displayDown()
{
    counter--;
    label->setString(std::to_string(counter));
}

void Counting::endGame()
{
    printf("ended");
}
