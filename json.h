#pragma once
#include <vector>
#include <string>
#include <unordered_map>

namespace json
{
	class Json;
	class ConvertableString
	{
	private:
		std::string elem;
		bool haveConverted = false;
	public:
		const int& asInt();
		const double& asDouble();
		const float& asFloat();
		const bool& asBool();
		const std::string& asString();
		std::vector<ConvertableString>& asArray();
		Json& asJson();
	public:
		bool checkString(const std::string& elem); // 判断elem是不是一个合法的json元素 
	public:
		ConvertableString(const std::string& elem);
		ConvertableString();
	};

	class Json
	{
	private:
		std::string json;
		std::unordered_map<std::string, ConvertableString> jsonMap;
	public:
		void parseJson();
		ConvertableString& operator[](const std::string& key);
	public:
		Json(const std::string& json);
		Json();
	};
}

