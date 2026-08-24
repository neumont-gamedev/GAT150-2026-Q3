#include "Enemy.h"
#include "Player.h"
#include "Engine.h"
#include "SpaceGame.h"

#include <iostream>

FACTORY_REGISTER(Enemy)

void Enemy::Update(float dt)
{
	Player* player = m_scene->GetActorByName<Player>("PlayerPrototype");
	if (player)
	{
		nu::Vector2 direction = player->GetTransform().position - m_transform.position;
		float rotation = direction.Angle();
		SetRotation(rotation * nu::RadToDeg);

		nu::Vector2 forward{ 1, 0 };
		forward = forward.Rotate(m_transform.rotation * nu::DegToRad);
		AddVelocity(forward * m_speed * dt);
	}

	nu::Particle particle;
	nu::Vector2 offset{ -20.0f, 0.0f };
	offset = offset.Rotate(m_transform.rotation * nu::DegToRad);
	particle.position = m_transform.position + offset;

	particle.texture = nu::Resources().Get<nu::Texture>("textures/particle.png", nu::Engine::Get().GetRenderer());
	particle.lifespan = nu::RandomFloat(0.15f, 0.5f);
	particle.velocity = nu::Vector2{ nu::RandomFloat(-100.0f, -30.0f), 0.0f }.Rotate((m_transform.rotation + nu::RandomInt(-30, 30)) * nu::DegToRad);

	nu::Engine::Get().GetPS().AddParticle(particle);


	Actor::Update(dt);
}

void Enemy::OnCollision(Actor* other)
{
	if (other->GetTag() == "PlayerBullet")
	{
		other->SetDestroyed();

		m_health -= 1.0f;
		if (m_health <= 0.0f)
		{
			SetDestroyed();

			((SpaceGame*)m_scene->GetGame())->AddPoints(m_points);

			nu::Engine::Get().GetAudio().PlaySound("explosion");
			// create particle explosion
			for (int i = 0; i < 100; i++)
			{
				nu::Particle particle;
				particle.position = m_transform.position;
				particle.texture = nu::Resources().Get<nu::Texture>("textures/particle.png", nu::Engine::Get().GetRenderer());
				particle.lifespan = nu::RandomFloat(0.15f, 0.75f);
				particle.velocity = { nu::RandomFloat(-600.0f, 600.0f), nu::RandomFloat(-600.0f, 600.0f) };

				nu::Engine::Get().GetPS().AddParticle(particle);
			}
		}
	}
}

void Enemy::Read(const nu::json::value_t& value)
{
	Actor::Read(value);

	JSON_READ_NAME(value, "points", m_points);
	JSON_READ_NAME(value, "health", m_health);
	JSON_READ_NAME(value, "speed", m_speed);
}