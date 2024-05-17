GaiaCookieManagerService::GaiaCookieManagerService(
    OAuth2TokenService* token_service,
    const std::string& source,
    SigninClient* signin_client)
    : token_service_(token_service),
      signin_client_(signin_client),
      external_cc_result_fetcher_(this),
      fetcher_backoff_(&kBackoffPolicy),
      fetcher_retries_(0),
      source_(source),
      external_cc_result_fetched_(false),
      list_accounts_stale_(true) {
  DCHECK(!source_.empty());
}
