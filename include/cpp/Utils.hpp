#pragma once

#include <string>
#include <vector>

namespace DexRobot
{

int64_t current_timestamp();
std::string timestamp_to_datetime_string(int64_t timestampInMs, bool as_utc=false);

void split(std::vector<std::string>& res, const std::string & str, const std::string & delimiter=" ");
void split(std::vector<std::string>& res, const std::string & str, char delimiter=' ');

} // End of namespace