    OnGetCheckConnectionInfoError(const GoogleServiceAuthError& error) {
  if (++helper_->fetcher_retries_ < kMaxFetcherRetries &&
      error.IsTransientError()) {
    helper_->fetcher_backoff_.InformOfRequest(false);
    gaia_auth_fetcher_timer_.Start(
        FROM_HERE, helper_->fetcher_backoff_.GetTimeUntilRelease(),
        this, &GaiaCookieManagerService::ExternalCcResultFetcher::Start);
    return;
  }

  CleanupTransientState();
  GetCheckConnectionInfoCompleted(false);
}
