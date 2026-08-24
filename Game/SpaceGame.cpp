#include "SpaceGame.h"
#include "Engine.h"
#include "Player.h"
#include "Enemy.h"

#include <memory>

using namespace nu;

bool SpaceGame::Initialize()
{
    Game::Initialize();

    m_scene = new Scene();
    m_scene->SetGame(this);
    m_scene->Load("data/scene.json");

    m_titleText = new TextRenderer(Resources().GetWithID<Font>("title_font", "fonts/airstrike.ttf", 128.0f));
    m_titleText->Create(Engine::Get().GetRenderer(), "XENON", Color{ 1.0f, 1.0f, 1.0f });

    m_scoreText = new TextRenderer(Resources().GetWithID<Font>("game_font", "fonts/airstrike.ttf", 32.0f));
    m_livesText = new TextRenderer(Resources().GetWithID<Font>("game_font", "fonts/airstrike.ttf", 32.0f));

    Engine::Get().GetAudio().AddSound("laser", "audio/laser.wav");
    Engine::Get().GetAudio().AddSound("explosion", "audio/explosion.wav");
    Engine::Get().GetAudio().AddSound("music", "audio/music.wav");
    Engine::Get().GetAudio().PlaySound("music", true);

    return true;
}

void SpaceGame::Update(float dt)
{
    switch (m_gameState)
    {
    case GameState::Title:
        if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE))
        {
            m_gameState = GameState::StartGame;
        }
        break;
    case GameState::StartGame:
        m_score = 0;
        m_lives = 3;
        m_spawnTime = 5.0f;
        m_stateTimer = 0.5f;
        m_gameState = GameState::StartLevel;
        break;
    case GameState::StartLevel:
        m_stateTimer -= dt;
        if (m_stateTimer <= 0)
        {
            m_scene->RemoveAllActors();
            SpawnPlayer();
            m_spawnTime = 5.0f;
            m_gameState = GameState::Game;
        }
        break;
    case GameState::Game:
        m_spawnTimer -= dt;
        if (m_spawnTimer <= 0.0f)
        {
            m_spawnTimer = m_spawnTime;
            SpawnEnemy();
            // increase difficulty
            m_spawnCount++;
            if (m_spawnCount > 5 && m_spawnTime >= 1.0f)
            {
                m_spawnCount = 0;
                m_spawnTime -= 0.5f;
            }
        }
        break;
    case GameState::GameOver:
        m_stateTimer -= dt;
        if (m_stateTimer <= 0)
        {
            m_scene->RemoveAllActors();
            m_gameState = GameState::Title;
        }
        break;
    default:
        break;
    }

    Game::Update(dt);
}

void SpaceGame::Draw(nu::Renderer& renderer)
{
    renderer.DrawTexture(*nu::Resources().Get<Texture>("textures/background.jpg", Engine::Get().GetRenderer()), Engine::Get().GetRenderer().GetWidth() * 0.5f, Engine::Get().GetRenderer().GetHeight() * 0.5f);

    switch (m_gameState)
    {
    case GameState::Title:
        // draw title
        m_titleText->Draw(renderer, 400, 400);
        break;
    case GameState::StartGame:
    case GameState::StartLevel:
    case GameState::Game:
        // draw score / lives
        m_scoreText->Create(renderer, "Score: " + std::to_string(m_score), { 1.0f, 1.0f, 1.0f });
        m_scoreText->Draw(renderer, 30, 30);

        m_livesText->Create(renderer, "Lives: " + std::to_string(m_lives), { 1.0f, 1.0f, 1.0f });
        m_livesText->Draw(renderer, (float)renderer.GetWidth() - 160, 30.0f);

        break;
    case GameState::GameOver:
        // draw game over
        break;
    default:
        break;
    }

    Game::Draw(renderer);
}

void SpaceGame::OnPlayerDead()
{
    m_lives--;
    m_gameState = (m_lives == 0) ? GameState::GameOver : GameState::StartLevel;

    m_stateTimer = 2.0f;
}

void SpaceGame::SpawnPlayer()
{
    auto actor = Factory::Instance().Create<Actor>("PlayerPrototype");
    m_scene->AddActor(std::move(actor));
}

void SpaceGame::SpawnEnemy()
{
    int enemyIndex = nu::RandomInt(2);
    if (enemyIndex == 0)
    {
        auto actor = Factory::Instance().Create<Actor>("EnemyPrototype");
        actor->SetPosition({ nu::RandomFloat(1024.0f), nu::RandomFloat(800.0f) });
        m_scene->AddActor(std::move(actor));
    }
    else if (enemyIndex == 1)
    {
        auto actor = Factory::Instance().Create<Actor>("EnemyPrototype");
        actor->SetPosition({ nu::RandomFloat(1024.0f), nu::RandomFloat(800.0f) });
        m_scene->AddActor(std::move(actor));
    }
}
