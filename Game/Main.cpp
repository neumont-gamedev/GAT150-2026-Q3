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
#include <fstream>


using namespace nu;

int main()
{
    SetWorkingDirectory("assets");

    {
        // read file (input file)
        std::ifstream file("data/text.txt");
        if (file.is_open())
        {
            std::string str;
            while (std::getline(file, str))
            {
                std::cout << str << std::endl;
            }
        }
        else
        {
            std::cout << "could not load: data/text.txt\n";
        }
        file.close();
    }

    {
        // write file (output file)
        std::ofstream file("data/text.txt", std::ios::app);
        if (file.is_open())
        {
            file << "Have a good day.\n";
        }
    }

    {
        // read / write (input / output file)
        std::fstream file("data/text.txt", std::ios::in | std::ios::out | std::ios::app);
        if (file.is_open())
        {
            // input
            file << "Add a line.\n";
            file.seekg(0);
            // output
            std::string str;
            while (std::getline(file, str))
            {
                std::cout << str << std::endl;
            }
        }
    }

    {
        std::string name;
        int score;
        bool isAlive;

        // save game data
        bool save = false;
        if (save)
        {
            name = "Raymond L Maple";
            score = 1234;
            isAlive = true;

            // save game data
            std::ofstream file("data/game.txt");
            if (file.is_open())
            {
                file << name << "\n";
                file << score << "\n";
                file << std::boolalpha << isAlive << "\n";
            }
        }

        // load game data
        bool load = true;
        if (load)
        {
            // read file (input file)
            std::ifstream file("data/game.txt");
            if (file.is_open())
            {
                std::getline(file, name);

                std::string str;
                std::getline(file, str);

                score = std::stoi(str);
                //file >> score;
                file >> std::boolalpha >> isAlive;
            }
        }

        // display game data
        std::cout << name << std::endl;
        std::cout << score << std::endl;
        std::cout << isAlive << std::endl;

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

