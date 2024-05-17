GaiaOAuthClient::GaiaOAuthClient(const OAuthProviderInfo& provider_info,
                                 net::URLRequestContextGetter* context_getter) {
  core_ = new Core(provider_info, context_getter);
}
