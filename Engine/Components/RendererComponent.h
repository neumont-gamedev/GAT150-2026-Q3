#pragma once
#include "Framework/Component.h"

namespace nu
{
	class RendererComponent : public Component
	{
	public:
		virtual void Draw(const class Renderer& renderer) = 0;
	};

	
}



