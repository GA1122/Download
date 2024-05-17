void BrowserPolicyConnector::RegisterForUserPolicy(
    const std::string& oauth_token) {
  if (user_data_store_.get())
    user_data_store_->SetOAuthToken(oauth_token);
}
