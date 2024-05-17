  const std::string& MaybeGetCachedToken(
      bool use_cached,
      const std::string& client_id,
      const std::string& app_id,
      const std::vector<std::string>& scopes) {
    if (!use_cached || host_->clock_->Now() >= last_token_expiry_) {
      last_token_.clear();
    }
    return last_token_;
  }
