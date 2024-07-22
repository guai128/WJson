#include <vector>
#include <string>
#include <iostream>
#include "json.h"

const std::string jsonPath = "test.json";

int main()
{
	std::string json = "{"
		"\"name\": \"yushifu\","
		"\"age\" : 18,"
		"\"isStudent\" : false,"
		"\"hobbies\" : [{"
		"\"street\": \"123 Main Street\","
		"\"city\" : \"弗洛里达\","
		"\"country\" : \"USA\""
		"}, \"abcv\", {"
		"\"street\": \"123 Main Street\"}],"
		"\"address\" : {"
		"\"street\": \"123 Main Street\","
		"\"city\" : \"弗洛里达\","
		"\"country\" : \"USA\""
		"},"
		"\"spouse\" : null"
		"}";

	json::Json j(json);

	for (auto& i : j["hobbies"].asArray())
	{
		std::cout << i.asString() << std::endl;
	}

	std::cout << j["hobbies"].asArray()[0].asJson()["country"].asString() << std::endl;
	std::cout << j["address"].asJson()["city"].asString() << std::endl;
}
