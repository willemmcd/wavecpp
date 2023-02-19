#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string get_current_datetime_string() {
  // Get the current time
  auto now = std::chrono::system_clock::now();
  auto now_time_t = std::chrono::system_clock::to_time_t(now);

  // Convert the time to a struct tm
  std::tm now_tm = *std::localtime(&now_time_t);

  // Create a string stream for formatting the date and time
  std::ostringstream datetime_stream;

  // Format the date and time
  datetime_stream << std::put_time(&now_tm, "%Y%m%d_%H%M%S");

  // Return the formatted date and time as a string
  return datetime_stream.str();
}
