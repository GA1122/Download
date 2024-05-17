void ChromotingInstance::HandleOnThirdPartyTokenFetched(
    const base::DictionaryValue& data) {
  std::string token;
  std::string shared_secret;
  if (!data.GetString("token", &token) ||
      !data.GetString("sharedSecret", &shared_secret)) {
    LOG(ERROR) << "Invalid onThirdPartyTokenFetched data.";
    return;
  }
  if (token_fetcher_proxy_.get()) {
    token_fetcher_proxy_->OnTokenFetched(token, shared_secret);
    token_fetcher_proxy_.reset();
  } else {
    LOG(WARNING) << "Ignored OnThirdPartyTokenFetched without a pending fetch.";
  }
}
