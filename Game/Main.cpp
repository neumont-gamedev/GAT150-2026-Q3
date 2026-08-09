#include "Engine.h"
#include "Player.h"
#include "Enemy.h"
#include "Assets.h"
#include "SpaceGame.h"

#include <fmod.hpp>

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <random>

using namespace nu;

int main()
{
    SetWorkingDirectory("assets");

    // file
    std::string text;
    nu::ReadTextFile("data/text.txt", text);
    std::cout << text << std::endl;

    // load the json data from a file
    std::string buffer;
    if (ReadTextFile("data/data.json", buffer))
    {
        // show the contents of the json file (debug)
        std::cout << buffer << std::endl;

        // create json document from the json file contents
        rapidjson::Document document;
        if (json::Load("data/data.json", document))
        {
            // read/show the data from the json file
            std::string name;
            int age;
            float speed;
            bool isAwake;
            nu::Vector2 position;
            nu::Vector3 color;

            // read the json data
            nu::json::Read(document, "name", name);
            nu::json::Read(document, "age", age);
            nu::json::Read(document, "speed", speed);
            nu::json::Read(document, "isAwake", isAwake);
            nu::json::Read(document, "position", position);
            nu::json::Read(document, "color", color);

            // show the data
            std::cout << name << " " << age << " " << speed << " " << isAwake << std::endl;
            std::cout << position.x << " " << position.y << std::endl;
            std::cout << color.r << " " << color.g << " " << color.b << " " << std::endl;
            
            // read the age data (int) from the json
            //int age;
            //json::Read(document, "age", age);
            // show the age data
            //std::cout << age << std::endl;
        }
    }


    return 0;

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

