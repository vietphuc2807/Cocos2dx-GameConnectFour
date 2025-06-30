    #include "Scene/LoadingEngine_Scene.h"
    #include "Scene/LoadingGame_Scene.h"

    USING_NS_CC;

    Scene* LoadingEngineScene::createScene()
    {
        return LoadingEngineScene::create();
    }

    bool LoadingEngineScene::init()
    {
        if ( !Scene::init() )
        {
            return false;
        }

	    // UI 
	    LoadingEngineScene::UIGameSetup();

        return true;
    }

    void LoadingEngineScene::UIGameSetup()
    {
        auto Size = Director::getInstance()->getVisibleSize();
        auto Origin = Director::getInstance()->getVisibleOrigin();

        // Logo
        logoEngine = Sprite::create("UI/LogoEngine.png");

        logoEngine->setPosition(Vec2(Size.width / 2 + Origin.x, Size.height / 2 + Origin.y));
        logoEngine->setScale(0.5f);
        logoEngine->setOpacity(50);
        logoEngine->getTexture()->setAntiAliasTexParameters();

        this->addChild(logoEngine);

	    // Logo animation
        auto fadeIn = FadeIn::create(2.0f);
        auto delay = DelayTime::create(0.8f);
        auto fadeOut = FadeOut::create(2.0f);
        auto goToLoadingGameScene = CallFunc::create([this]() {
            auto scene = LoadingGameScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene));
            });

        auto sequence = Sequence::create(fadeIn, delay, fadeOut, goToLoadingGameScene, nullptr);

        logoEngine->runAction(sequence);
    }