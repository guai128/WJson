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
		"\"hobbies\" : [\"singing\", \"jumping\", \"rapping\", \"basketball\"],"
		"\"address\" : {"
		"\"street\": \"123 Main Street\","
		"\"city\" : \"弗洛里达\","
		"\"country\" : \"USA\""
		"},"
		"\"spouse\" : null"
		"}";

	json::Json j(json);

	std::cout << j["hobbies"].asArray()[3].asString() << std::endl;
	std::cout << j["address"].asJson()["city"].asString() << std::endl;
}
