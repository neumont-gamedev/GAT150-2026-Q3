#include "Player.h"
#include "Bullet.h"
#include "Assets.h"
#include "Renderer.h"
#include "Engine.h"
#include "SpaceGame.h"

void Player::Update(float dt)
{
	// movement
	float thrust = 0.0f;
	if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) thrust =  m_speed;
	if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) thrust = -m_speed;


	float rotate = 0.0f;
	if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) rotate = -180.0f;
	if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) rotate = +180.0f;

	SetRotation(m_transform.rotation + rotate * dt);

	nu::Vector2 forward{ 1, 0 }; // ->
	nu::Vector2 velocity = forward.Rotate(m_transform.rotation * nu::DegToRad) * thrust;
	AddVelocity(velocity * dt);

	// particle system
	if (thrust)
	{
		nu::Particle particle;
		nu::Vector2 offset{ -20.0f, 0.0f };
		offset = offset.Rotate(m_transform.rotation * nu::DegToRad);
		particle.position = m_transform.position + offset;

		nu::Color colors[3] = { { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f } };
		particle.color = colors[nu::RandomInt(3)];
		particle.lifespan = nu::RandomFloat(0.5f, 1.5f);
		particle.velocity = nu::Vector2{ nu::RandomFloat(-30.0f, -100.0f), 0.0f}.Rotate((m_transform.rotation + nu::RandomInt(-30, 30)) * nu::DegToRad);

		nu::Engine::Get().GetPS().AddParticle(particle);
	}

	// fire
	m_fireTimer -= dt;
	if (m_fireTimer <= 0.0f && nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_SPACE))
	{
		bool burst = (m_fireTimer <= -1.0f);

		m_fireTimer = 0.25f;
		nu::Engine::Get().GetAudio().PlaySound("laser", true);

		BulletDesc desc;
		desc.name = "Bullet";
		desc.tag = "PlayerBullet";
		desc.model = assets::bulletModel;
		desc.transform = m_transform;
		desc.transform.scale = 5.0f;
		desc.speed = 1000.0f;
		desc.lifespan = 1.0f;
		
		// create bullets
		m_scene->AddActor(std::move(std::make_unique<Bullet>(desc)));

		if (burst)
		{
			desc.transform.rotation += 10.0f;
			m_scene->AddActor(std::move(std::make_unique<Bullet>(desc)));

			desc.transform.rotation -= 20.0f;
			m_scene->AddActor(std::move(std::make_unique<Bullet>(desc)));
		}
	}

	// bullet time
	if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_X))
	{
		nu::Engine::Get().GetTime().SetTimeScale(0.5f);
	}
	else
	{
		nu::Engine::Get().GetTime().SetTimeScale(1.0f);
	}


	Actor::Update(dt);
}

void Player::OnCollision(Actor* other)
{
	if (other->GetName() == "Enemy")
	{
		SetDestroyed();
		other->SetDestroyed();

		nu::Engine::Get().GetAudio().PlaySound("explosion");
		((SpaceGame*)m_scene->GetGame())->OnPlayerDead();
	}
}
