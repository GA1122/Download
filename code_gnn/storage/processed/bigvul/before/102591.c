void EnterpriseEnrollmentScreen::OnIssueAuthTokenFailure(
    const std::string& service,
    const GoogleServiceAuthError& error) {
  if (service != GaiaConstants::kDeviceManagementService) {
    NOTREACHED() << service;
    UMA_HISTOGRAM_ENUMERATION(policy::kMetricEnrollment,
                              policy::kMetricEnrollmentOtherFailed,
                              policy::kMetricEnrollmentSize);
    return;
  }

  HandleAuthError(error);
}
