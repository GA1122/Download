  void GetAccessToken(bool use_cached,
                      const std::string& client_id,
                      const std::string& app_id,
                      const std::vector<std::string>& scopes,
                      mojom::DriveFsDelegate::GetAccessTokenCallback callback) {
    DCHECK_CALLED_ON_VALID_SEQUENCE(host_->sequence_checker_);
    if (get_access_token_callback_) {
      std::move(callback).Run(mojom::AccessTokenStatus::kTransientError, "");
      return;
    }
    DCHECK(!mint_token_flow_);
    const std::string& token =
        MaybeGetCachedToken(use_cached, client_id, app_id, scopes);
    if (!token.empty()) {
      std::move(callback).Run(mojom::AccessTokenStatus::kSuccess, token);
      return;
    }
    get_access_token_callback_ = std::move(callback);
    mint_token_flow_ =
        host_->delegate_->CreateMintTokenFlow(this, client_id, app_id, scopes);
    DCHECK(mint_token_flow_);
    GetIdentityManager().GetPrimaryAccountWhenAvailable(base::BindOnce(
        &AccountTokenDelegate::AccountReady, base::Unretained(this)));
  }
