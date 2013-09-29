#include "GameLayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* GameLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameLayer *layer = GameLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

GameLayer::~GameLayer()
{
    CC_SAFE_RELEASE(_players);
}
// on "init" you need to initialize your instance
bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
 
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(GameLayer::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );
    
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Thonburi", 34);

    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    // position the label on the center of the screen
    pLabel->setPosition( ccp(size.width / 2, size.height - 20) );

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    _player1Score = 0;
    _player2Score = 0;
    _screenSize = CCDirector::sharedDirector() -> getWinSize();
    
    CCSprite * court = CCSprite::create("court.png");
    court->setPosition(ccp(_screenSize.width *0.5, _screenSize.height * 0.5));
    this -> addChild(court);
    
    _player1 = GameSprite::gameSpriteWithFile("mallet.png");
    _player1 -> setPosition(ccp(_screenSize.width * 0.5, _player1->radius() * 2));
    this -> addChild(_player1);
    
    _player2 = GameSprite::gameSpriteWithFile("mallet.png");
    _player2 -> setPosition(ccp(_screenSize.width * 0.5, _screenSize.height - _player2->radius() * 2));
    this -> addChild(_player2);
    
    _ball = GameSprite::gameSpriteWithFile("puck.png");
    _ball -> setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5 - 2 * _ball->radius()));
    this-> addChild(_ball);
    
    _players = CCArray::create(_player1, _player2, NULL);
    _players->retain();
    
    _player1ScoreLabel = CCLabelTTF::create("0", "Arial", 60);
    _player1ScoreLabel -> setPosition(ccp(_screenSize.width - 60, _screenSize.height * 0.5 - 80));
    _player1ScoreLabel -> setRotation(90);
    this->addChild(_player1ScoreLabel);
    
    _player2ScoreLabel = CCLabelTTF::create("0", "Arial", 60);
    _player2ScoreLabel -> setPosition(ccp(_screenSize.width - 60, _screenSize.height * 0.5 + 80));
    _player2ScoreLabel -> setRotation(90);
    this->addChild(_player2ScoreLabel);
    
    this->setTouchEnabled(true);
    this->schedule(schedule_selector(GameLayer::update));
    
    return true;
}

void GameLayer::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void GameLayer::ccTouchesBegan(CCSet * pTouches, CCEvent* event) {
    CCSetIterator i;
    CCTouch* touch;
    CCPoint tap;
    GameSprite * player;
    for (i = pTouches->begin(); i != pTouches->end(); i++){
        touch = (CCTouch*) (*i);
        if (touch) {
            tap = touch->getLocation();
            for (int p = 0; p<_players->count(); p++) {
                player = (GameSprite *) _players-> objectAtIndex(p);
                if (player->boundingBox().containsPoint(tap)) {
                    player->setTouch(touch);
                    //CCLOG("%d Begin %f %f", p, tap.x, tap.y);
                }
            }
        }
    }
}

void GameLayer::ccTouchesMoved(CCSet * pTouches, CCEvent* event) {
    CCSetIterator i;
    CCTouch* touch;
    CCPoint tap;
    GameSprite * player;
    
    for (i = pTouches->begin(); i != pTouches->end(); i++){
        touch = (CCTouch*) (*i);
        if (touch) {
            tap = touch->getLocation();
            for (int p = 0; p<_players->count(); p++) {
                player = (GameSprite *) _players-> objectAtIndex(p);
                //CCLOG("%d Move %f %f", p, tap.x, tap.y);
                if (player->getTouch() != NULL && player->getTouch() == touch) {
                    CCPoint nextPosition = tap;
                    if (nextPosition.x < player->radius())
                        nextPosition.x = player->radius();
                    if (nextPosition.x > _screenSize.width - player->radius())
                        nextPosition.x = _screenSize.width - player->radius();
                    if (nextPosition.y <  player->radius())
                        nextPosition.y = player->radius();
                    if (nextPosition.y > _screenSize.height - player->radius())
                        nextPosition.y = _screenSize.height - player -> radius();
                    
                    if (player->getPositionY() < _screenSize.height * 0.5f){
                        if (nextPosition.y > _screenSize.height * 0.5 - player->radius())
                            nextPosition.y = _screenSize.height * 0.5 - player->radius();
                    }
                    else {
                        if (nextPosition.y < _screenSize.height * 0.5 + player->radius())
                            nextPosition.y = _screenSize.height * 0.5 + player->radius();
                    }
                    player->setNextPosition(nextPosition);
                    player->setVector(ccp(tap.x - player->getPositionX(), tap.y - player->getPositionY()));
                    
                }
            }
        }
    }
}

void GameLayer::ccTouchesEnded(CCSet * pTouches, CCEvent* event){
    CCSetIterator i;
    CCTouch* touch;
    CCPoint tap;
    GameSprite * player;
    
    for (i = pTouches->begin(); i != pTouches->end(); i++){
        touch = (CCTouch*) (*i);
        if (touch) {
            tap = touch->getLocation();
            for (int p = 0; p<_players->count(); p++) {
                player = (GameSprite *) _players-> objectAtIndex(p);
                if (player->getTouch() != NULL && player -> getTouch() == touch) {
                    player->setTouch(NULL);
                    player->setVector(ccp(0,0));
                }
            }
        }
    }
}

void GameLayer::update(float dt) {
    CCPoint ballNextPosition = _ball -> getNextPosition();
    CCPoint ballVector = _ball->getVector();
    ballVector = ccpMult(ballVector, 0.98f);
    ballNextPosition.x += ballVector.x;
    ballNextPosition.y += ballVector.y;
    
    GameSprite * player;
    CCPoint playerNextPosition;
    CCPoint playerVector;
    
    float squared_radii = pow(_player1->radius() + _ball -> radius(),2);
    //CCLOG("update");
    for (int p=0; p<_players->count();p++) {
        player = (GameSprite *) _players->objectAtIndex(p);
        playerNextPosition = player->getNextPosition();
        playerVector = player->getVector();
        
        float diffx = ballNextPosition.x - player->getPositionX();
        float diffy = ballNextPosition.y - player->getPositionY();
        
        float distance1 = pow(diffx,2) + pow(diffy,2);
        float distance2 = pow(_ball->getPositionX() - playerNextPosition.x,2) + pow(_ball->getPositionY() - playerNextPosition.y,2);
        
        if (distance1 <= squared_radii || distance2 <=squared_radii)
        {
            float mag_ball = pow(ballVector.x, 2) + pow(ballVector.y,2);
            float mag_player = pow(playerVector.x,2) + pow(playerVector.y,2);
            float force = sqrt(mag_ball+mag_player);
            float angle = atan2(diffy,diffx);
            
            ballVector.x = force * cos(angle);
            ballVector.y = force * sin(angle);
            
            ballNextPosition.x = playerNextPosition.x + (player->radius() + _ball->radius() + force) * cos(angle);
            ballNextPosition.y = playerNextPosition.y + (player->radius() + _ball->radius() + force) * sin(angle);
            SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
            
        }
    }
    if (ballNextPosition.x < _ball->radius())
    {
        ballNextPosition.x = _ball->radius();
        ballVector.x *= -0.8f;
        SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
    }
    if (ballNextPosition.x > _screenSize.width - _ball->radius()){
        ballNextPosition.x = _screenSize.width - _ball->radius();
        ballVector.x *= -0.8f;
        SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
    }
    
    if (ballNextPosition.y > _screenSize.height - _ball->radius()){
        if (_ball->getPosition().x < _screenSize.width * 0.5f - GOAL_WIDTH * 0.5f || _ball->getPosition().x > _screenSize.width * 0.5f + GOAL_WIDTH * 0.5f) {
            ballNextPosition.y = _screenSize.height - _ball->radius();
            ballVector.y *= 0.8f;
            SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
        }
    }
    if (ballNextPosition.y < _ball->radius()){
        if (_ball->getPosition().x < _screenSize.width * 0.5f - GOAL_WIDTH * 0.5f || _ball->getPosition().x > _screenSize.width * 0.5f + GOAL_WIDTH * 0.5f) {
            ballNextPosition.y = _ball->radius();
            ballVector.y *= -0.8f;
            SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
        }
    }
    
    _ball->setVector(ballVector);
    _ball->setNextPosition(ballNextPosition);
    
    //check for goals!
    if (ballNextPosition.y < -_ball->radius() * 2){
        this->playerScore(2);
    }
    if(ballNextPosition.y > _screenSize.height + _ball->radius() * 2)
    {
        this->playerScore(1);
    }
    _player1->setPosition(_player1->getNextPosition());
    _player2->setPosition(_player2->getNextPosition());
    _ball->setPosition(_ball->getNextPosition());
}

void GameLayer::playerScore(int player) {
    SimpleAudioEngine::sharedEngine()->playEffect("score.wav");
    _ball->setVector(ccp(0,0));
    char score_buffer[10];
    if (player == 1){
        _player1Score++;
        sprintf(score_buffer, "%i", _player1Score);
        _player1ScoreLabel->setString(score_buffer);
        _ball->setNextPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5 + 2 * _ball->radius()));
    } else {
        _player2Score++;
        sprintf(score_buffer, "%i", _player2Score);
        _player2ScoreLabel->setString(score_buffer);
        _ball->setNextPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5 + 2 * _ball->radius()));
    }
    
    _player1->setPosition(ccp(_screenSize.width * 0.5, _player1->radius() * 2));
    _player2->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height - _player2->radius() * 2));
    _player1->setTouch(NULL);
    _player2->setTouch(NULL);
}

