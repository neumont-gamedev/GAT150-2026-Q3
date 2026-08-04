#include "SpaceGame.h"
#include "Engine.h"
#include "Player.h"
#include "Enemy.h"
#include "Assets.h"

using namespace nu;

bool SpaceGame::Initialize()
{
    Game::Initialize();

    m_scene = new Scene();
    m_scene->SetGame(this);

    m_titleFont = new Font();
    m_titleFont->Load("fonts/airstrike.ttf", 64);

    m_titleText = new Text(m_titleFont);
    m_titleText->Create(Engine::Get().GetRenderer(), "XENON", Color{ 1.0f, 1.0f, 1.0f });

    m_gameFont = new Font();
    m_gameFont->Load("fonts/airstrike.ttf", 32);

    m_scoreText = new Text(m_gameFont);
    m_livesText = new Text(m_gameFont);

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
    PlayerDesc playerDesc;
    playerDesc.name = "Player";
    playerDesc.model = assets::playerModel;
    playerDesc.transform = Transform{ Vector2{ 640.0f, 512.0f }, 0.0f, 15.0f };
    playerDesc.velocity = Vector2{ 0.0f, 0.0f };
    playerDesc.damping = 3.0f;
    playerDesc.speed = 2000.0f;

    std::unique_ptr<Player> player = std::make_unique<Player>(playerDesc);
    m_scene->AddActor(std::move(player));
}

void SpaceGame::SpawnEnemy()
{
    int enemyIndex = nu::RandomInt(2);
    if (enemyIndex == 0)
    {
        EnemyDesc enemyDesc;
        enemyDesc.name = "Enemy";
        enemyDesc.model = assets::enemyModel;
        enemyDesc.transform = Transform{ Vector2{ nu::RandomFloat((float)nu::Engine::Get().GetRenderer().GetWidth()), nu::RandomFloat((float)nu::Engine::Get().GetRenderer().GetHeight())}, 90.0f, 10.0f };
        enemyDesc.speed = RandomFloat(200.0f, 500.0f);
        enemyDesc.damping = 3.0f;
        enemyDesc.health = 2.0f;
        enemyDesc.points = 100;

        m_scene->AddActor(std::move(std::make_unique<Enemy>(enemyDesc)));
    }
    else if (enemyIndex == 1)
    {
        EnemyDesc enemyDesc;
        enemyDesc.name = "Enemy";
        enemyDesc.model = assets::enemyBossModel;
        enemyDesc.transform = Transform{ Vector2{ nu::RandomFloat((float)nu::Engine::Get().GetRenderer().GetWidth()), nu::RandomFloat((float)nu::Engine::Get().GetRenderer().GetHeight())}, 90.0f, 10.0f };
        enemyDesc.speed = RandomFloat(300.0f, 600.0f);
        enemyDesc.damping = 3.0f;
        enemyDesc.health = 5.0f;
        enemyDesc.points = 500;

        m_scene->AddActor(std::move(std::make_unique<Enemy>(enemyDesc)));
    }
}
