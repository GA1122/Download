void GaiaCookieManagerService::ExternalCcResultFetcher::Start() {
  m_external_cc_result_start_time_ = base::Time::Now();

  CleanupTransientState();
  results_.clear();
  helper_->gaia_auth_fetcher_.reset(
      helper_->signin_client_->CreateGaiaAuthFetcher(
          this, helper_->GetDefaultSourceForRequest(),
          helper_->request_context()));
  helper_->gaia_auth_fetcher_->StartGetCheckConnectionInfo();

  timer_.Start(FROM_HERE, base::TimeDelta::FromSeconds(5), this,
               &GaiaCookieManagerService::ExternalCcResultFetcher::Timeout);
}
