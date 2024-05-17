void GaiaOAuthClient::RefreshToken(const OAuthClientInfo& oauth_client_info,
                                   const std::string& refresh_token,
                                   Delegate* delegate) {
  return core_->RefreshToken(oauth_client_info,
                             refresh_token,
                             delegate);
}
