  std::string status_and_headers() const {
    return std::string(status) + "\n" + std::string(headers);
  }
