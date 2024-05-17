  void GotChromeAccessToken(const base::Optional<std::string>& access_token,
                            base::Time expiration_time,
                            const GoogleServiceAuthError& error) {
    DCHECK_CALLED_ON_VALID_SEQUENCE(host_->sequence_checker_);
    if (!access_token) {
      OnMintTokenFailure(error);
      return;
    }
    mint_token_flow_->Start(host_->delegate_->GetURLLoaderFactory(),
                            *access_token);
  }
