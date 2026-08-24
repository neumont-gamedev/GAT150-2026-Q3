#pragma once

#include "Core/Random.h"
#include "Core/File.h"
#include "Core/StringUtils.h"
#include "Core/Factory.h"
#include "Core/GameTime.h"

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Transform.h"
#include "Math/MathUtils.h"
#include "Math/Rect.h"

#include "Serialization/Json.h"

#include "Renderer/Renderer.h"
#include "Renderer/TextRenderer.h"
#include "Renderer/Texture.h"
#include "Renderer/ParticleSystem.h"

#include "Resources/ResourceManager.h"

#include "Framework/Actor.h"
#include "Framework/Scene.h"
#include "Framework/Game.h"

#include "Input/Input.h"
#include "Audio/Audio.h"
#include "Physics/Physics.h"

namespace nu
{
	class Engine
	{
	public:
		static Engine& Get() { static Engine engine; return engine; }

		bool Initialize();
		void Shutdown();

		void Update();

		Input& GetInput() { return m_input; }
		Renderer& GetRenderer() { return m_renderer; }
		Audio& GetAudio() { return m_audio; }
		Time& GetTime() { return m_time; }
		ParticleSystem& GetPS() { return m_particleSystem; }
		Physics& GetPhysics() { return m_physics; }

		Engine(const Engine&) = delete;
		Engine& operator = (const Engine&) = delete;

	private:
		Engine() = default;

	private:
		Input m_input;
		Renderer m_renderer;
		Audio m_audio;
		ParticleSystem m_particleSystem;
		Physics m_physics;

		Time m_time;
	};	
}
