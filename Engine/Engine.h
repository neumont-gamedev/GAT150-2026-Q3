#pragma once

#include "Random.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Transform.h"
#include "MathUtils.h"
#include "File.h"
#include "Json.h"
#include "StringUtils.h"

#include "Text.h"
#include "Texture.h"

#include "Factory.h"
#include "ResourceManager.h"

// systems
#include "Renderer.h"
#include "Input.h"
#include "Audio.h"
#include "GameTime.h"
#include "ParticleSystem.h"

// framework
#include "Actor.h"
#include "Scene.h"
#include "Game.h"

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

		Engine(const Engine&) = delete;
		Engine& operator = (const Engine&) = delete;

	private:
		Engine() = default;

	private:
		Input m_input;
		Renderer m_renderer;
		Audio m_audio;
		ParticleSystem m_particleSystem;

		Time m_time;
	};	
}
