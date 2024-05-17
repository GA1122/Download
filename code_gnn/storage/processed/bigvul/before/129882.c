  bool TraceHasMatchingString(const char* str) {
    return last_file_contents_.find(str) != std::string::npos;
  }
