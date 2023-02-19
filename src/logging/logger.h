#include <fstream>
#include <string>
#include <filesystem>
#include "../utils/datetime.h"

bool isFileSizeLargerThan(const std::string& filename, std::uintmax_t size) {
  return std::filesystem::file_size(filename) > size;
}

std::string getInfoType() {
  return "INFO";
}

std::string getWarningType() {
  return "WARNING";
}

std::string getErrorType() {
  return "ERROR";
}

void writeToLog(const std::string& filename, const std::string& msgtype, const std::string& content) {

  constexpr std::uintmax_t kMaxFileSize = 1 * 1024 * 1024; // 5MB

  auto log_directory = std::filesystem::path(filename).parent_path();

  if (!std::filesystem::exists(log_directory)) {
    std::filesystem::create_directories(log_directory);
  }

  //Write the content to the terminal
  std::string currentDateTime = get_current_datetime_string();
  std::cout << currentDateTime << " " << msgtype << " | " << content << std::endl;
  // Write the content to the current log file
  std::ofstream ofs(filename, std::ios::app);
  if (ofs) {
    ofs << currentDateTime << " " << msgtype << " | " << content << std::endl;
  }

  // Check if the current log file exceeds the maximum size
  if (isFileSizeLargerThan(filename, kMaxFileSize)) {
    // Rename the current log file
    std::filesystem::path logPath(filename);
    currentDateTime = get_current_datetime_string();
   

    std::string newPath = logPath.parent_path().string() + "/"
            + logPath.stem().string() + "_"
            + currentDateTime + logPath.extension().string();
    std::filesystem::rename(filename, newPath);
  }

  
}

