#include "pch.h"
#include "SpriteAnimationRendererComponent.h"
#include "Core/Factory.h"
#include "Resources/ResourceManager.h"
#include "Renderer/TextureFrames.h"
#include "Math/MathUtils.h"
#include "Math/Transform.h"
#include "Framework/Actor.h"

#include "Engine.h"

namespace nu
{
	FACTORY_REGISTER(SpriteAnimationRendererComponent)


	void SpriteAnimationRendererComponent::Update(float dt)
	{
		if (!m_textureFrames) return;

		m_frameTimer += dt;
		float frameTime = 1.0f / m_framesPerSecond;

		// increase frame while frame timer is greater than frame time
		while (m_frameTimer >= frameTime)
		{
			m_frame++;
			if (m_loop)
			{
				// loop frames
				m_frame = Wrap(0u, m_textureFrames->GetTotalFrames() - 1, m_frame);
			}
			else
			{
				// stop on last frame
				m_frame = Clamp(0u, m_textureFrames->GetTotalFrames() - 1, m_frame);
			}

			m_frameTimer -= frameTime;
		}
	}

	void SpriteAnimationRendererComponent::Draw(const Renderer& renderer)
	{
		if (!m_textureFrames) return;

		auto transform = GetOwner()->GetTransform();

		renderer.DrawTexture(
			*m_textureFrames->GetTexture(), 
			m_textureFrames->GetFrameRect(m_frame),
			transform.position.x,
			transform.position.y,
			transform.rotation,
			transform.scale);
	}

	void SpriteAnimationRendererComponent::Read(const json::value_t& value)
	{
		RendererComponent::Read(value);

		JSON_READ_NAME_REQ(value, "frames_per_second", m_framesPerSecond);
		JSON_READ_NAME(value, "loop", m_loop);

		std::string texture_frames;
		JSON_READ_REQ(value, texture_frames);

		if (!texture_frames.empty())
		{
			m_textureFrames = Resources().Get<TextureFrames>(texture_frames, Engine::Instance().GetRenderer());
			if (!m_textureFrames)
			{
				std::cerr << "Could not load texture frames: " << texture_frames << std::endl;
			}
		}
	}
}
