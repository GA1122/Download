  void OnLoginSuccess(const std::string& username,
                      const std::string& password,
                      const GaiaAuthConsumer::ClientLoginResult& credentials,
                      bool pending_requests,
                      bool using_oauth) {
    pending_requests_ = pending_requests;
    if (!profile_prepared_) {
      chromeos::LoginUtils::Get()->PrepareProfile(username,
                                                  std::string(),
                                                  password,
                                                  credentials,
                                                  pending_requests,
                                                  using_oauth,
                                                  false,
                                                  this);
    } else if (!pending_requests) {
      delete this;
    }
  }
