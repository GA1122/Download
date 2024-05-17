  void AddAuthTokenManually(const std::string& service,
                            const std::string& value) {
    if (token_web_data_)
      token_web_data_->SetTokenForService(service, value);
  }
