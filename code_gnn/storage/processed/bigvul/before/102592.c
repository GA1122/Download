void EnterpriseEnrollmentScreen::OnIssueAuthTokenSuccess(
    const std::string& service,
    const std::string& auth_token) {
  if (service != GaiaConstants::kDeviceManagementService) {
    UMA_HISTOGRAM_ENUMERATION(policy::kMetricEnrollment,
                              policy::kMetricEnrollmentOtherFailed,
                              policy::kMetricEnrollmentSize);
    NOTREACHED() << service;
    return;
  }

  scoped_ptr<GaiaAuthFetcher> auth_fetcher(auth_fetcher_.release());

  RegisterForDevicePolicy(auth_token,
                          policy::BrowserPolicyConnector::TOKEN_TYPE_GAIA);
}
