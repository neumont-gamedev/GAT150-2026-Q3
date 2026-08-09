#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include <rapidjson/document.h>
#include <string>

namespace nu::json
{
	bool Load(const std::string& filename, rapidjson::Document& document);

	// read json data
	bool Read(const rapidjson::Value& value, const std::string& name, int& data);
	bool Read(const rapidjson::Value& value, const std::string& name, float& data);
	bool Read(const rapidjson::Value& value, const std::string& name, bool& data);
	bool Read(const rapidjson::Value& value, const std::string& name, std::string& data);
	bool Read(const rapidjson::Value& value, const std::string& name, Vector2& data);
	bool Read(const rapidjson::Value& value, const std::string& name, Vector3& data);
}
