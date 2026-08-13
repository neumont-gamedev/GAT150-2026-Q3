#include "Engine.h"
#include "Player.h"
#include "Enemy.h"
#include "Assets.h"
#include "SpaceGame.h"
#include "Bullet.h"

#include <fmod.hpp>

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <random>
#include <fstream>

using namespace nu;

int main()
{
    // CAN'T TOUCH THIS!
    SetWorkingDirectory("assets");

    Factory::Instance().Register<Actor>("Actor");
    Factory::Instance().Register<Object>("Object");
    Factory::Instance().Register<Player>("Player");
    Factory::Instance().Register<Enemy>("Enemy");
    Factory::Instance().Register<Bullet>("Bullet");

    /*
        
    auto actor = Factory::Instance().Create<Actor>("Actor");
    std::cout << actor->IsActive() << std::endl;

    auto object = Factory::Instance().Create("Object");
    std::cout << object->IsActive() << std::endl;

    auto player = Factory::Instance().Create<Player>("Player");

    json::document_t document;
    if (json::Load("data/scene.json", document))
    {
        player->Read(document);

        std::cout << player->GetName() << std::endl;
        std::cout << player->GetTag() << std::endl;

        std::cout << player->GetTransform().rotation << std::endl;
        std::cout << player->GetSpeed() << std::endl;
    }

    return 0;
    */


    // INITIALIZATION
    Engine::Get().Initialize();

    SpaceGame game;
    game.Initialize();

    // MAIN LOOP
    bool quit = false;
    while (!quit) 
    {
        // UPDATE
        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_EVENT_QUIT) 
            {
                quit = true;
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE)
            {
                quit = true;
            }
        }

        // ENGINE
        Engine::Get().Update();
        float dt = Engine::Get().GetTime().GetDeltaTime();

        // GAME
        game.Update(dt);

        // RENDER
        Engine::Get().GetRenderer().SetColor(0.0f, 0.0f, 0.0f);
        Engine::Get().GetRenderer().Clear();

        game.Draw(Engine::Get().GetRenderer());
        Engine::Get().GetPS().Draw(Engine::Get().GetRenderer());

        Engine::Get().GetRenderer().Present();
    }

    // SHUTDOWN
    Engine::Get().Shutdown();    

    return 0;
}

