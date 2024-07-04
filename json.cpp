#include "json.h"
#include <stdexcept>
#include <stack>

using json::ConvertableString;
using json::Json;

// ConvertableString
ConvertableString::ConvertableString(const std::string& str)
{
	for (auto c : str)
	{
		if (c != ' ' && c != '\t' && c != '\n')
			this->elem.push_back(c);
	}
}

ConvertableString::ConvertableString()
{
	
}

const int& ConvertableString::asInt()
{
	static int result;
	if (haveConverted) return result;
	try
	{
		result = stoi(this->elem);
		haveConverted = true;
	}
	catch (const std::exception&)
	{
		throw std::invalid_argument("Cannot convert \"" + elem + "\" to bool");
	}
	return result;
}

const double& ConvertableString::asDouble()
{
	static double result;
	if (haveConverted) return result;
	try
	{
		result = stod(this->elem);
		haveConverted = true;
	}
	catch (const std::exception&)
	{
		throw std::invalid_argument("Cannot convert \"" + elem + "\" to bool");
	}
	return result;
}

const float& ConvertableString::asFloat()
{
	static float result;
	if (haveConverted) return result;
	try
	{
		result = stof(this->elem);
		haveConverted = true;
	}
	catch (const std::exception&)
	{
		throw std::invalid_argument("Cannot convert \"" + elem + "\" to bool");
	}
	return result;
}

const bool& ConvertableString::asBool()
{
	static bool result;
	if (haveConverted) return result;

	if (this->elem == "true")
		result = true;
	else if (this->elem == "false")
		result = false;
	else
		throw std::invalid_argument("Cannot convert \"" + elem + "\" to bool");

	haveConverted = true;
	return result;
}

const std::string& ConvertableString::asString()
{
	return elem;
}

std::vector<ConvertableString>& ConvertableString::asArray()
{
	static std::vector<ConvertableString> result;
	if (haveConverted) return result;

	if (elem.front() != '[' || elem.back() != ']')
		throw std::invalid_argument("Cannot convert \"" + elem + "\" to array");
	
	size_t n = elem.size();
	std::string tmp;
	for (int i = 1; i < n - 1; i++)
	{
		if (elem[i] == ',')
		{
			if (!checkString(tmp))
				throw std::invalid_argument(elem + "is not a legal json data");
			if (tmp.front() == '\"')
				tmp = tmp.substr(1, tmp.size() - 2);

			result.push_back(ConvertableString(tmp));
			tmp.clear();
		}
		else
			tmp.push_back(elem[i]);
	}

	if (!tmp.empty())
	{
		if (!checkString(tmp))
			throw std::invalid_argument(elem + "is not a legal json data");
		if (tmp.front() == '\"')
			tmp = tmp.substr(1, tmp.size() - 2);
		result.push_back(ConvertableString(tmp));
	}

	return result;
}

Json& ConvertableString::asJson()
{
	static Json result;
	if (haveConverted) return result;

	if (elem.front() != '{' || elem.back() != '}')
		throw std::invalid_argument("Cannot convert \"" + elem + "\" to json");

	result = Json(elem);
	return result;
}

bool ConvertableString::checkString(const std::string& elem)
{
	if (elem.empty())
		return false;
	if (elem.front() == '\"' && elem.back() != '\"')
		return false;
	if (elem.front() != '\"' && elem.back() == '\"')
		return false;
	return true;
}

// Json
Json::Json(const std::string& json)
{
	for (auto c : json)
	{
		if (c != ' ' && c != '\t' && c != '\n')
			this->json.push_back(c);
	}
	this->parseJson();
}

Json::Json()
{
	
}

void Json::parseJson()
{
	if (json.empty() || json.front() != '{' || json.back() != '}')
		throw std::invalid_argument("Json format error");
	
	size_t n = json.size(), pos = 1;

	char wait = 0;
	while (pos < n - 1)
	{
		if (json[pos] != '"') 
			throw std::invalid_argument("Json format error");
		pos++;
		std::string key;
		while (pos < n - 1 && json[pos] != '"')
		{
			key.push_back(json[pos]);
			pos++;
		}
		pos++;

		if (pos >= n - 1 || json[pos] != ':')
			throw std::invalid_argument("Json format error");
		pos++;

		if (pos >= n - 1)
			throw std::invalid_argument("Json format error");

		if (json[pos] == '[') wait = ']';
		else if (json[pos] == '{') wait = '}';
		else wait = 0;

		std::string value;
		while (pos < n - 1 && ! (!wait && json[pos] == ','))
		{
			value.push_back(json[pos]);
			pos++;
		
			if (json[pos - 1] == wait)
				break;
		}

		if (value.empty() || 
			(value.front() == '\"' && value.back() != '\"') || 
			(value.front() != '\"' && value.back() == '\"'))
			throw std::invalid_argument("Json format error");
		if (value.front() == '\"' && value.back() == '\"')
			value = value.substr(1, value.size() - 2);

		jsonMap[key] = ConvertableString(value);

		if (pos < n - 1 && json[pos] == ',')
			pos++;
	}
}

ConvertableString& Json::operator[](const std::string& key)
{
	if (! jsonMap.count(key))
		throw std::invalid_argument("Cannot find key \"" + key + "\" in json");
	return jsonMap[key];
}
