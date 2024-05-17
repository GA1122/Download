void EnterpriseEnrollmentScreen::OnClientLoginSuccess(
    const ClientLoginResult& result) {
  auth_fetcher_->StartIssueAuthToken(
          result.sid, result.lsid, GaiaConstants::kDeviceManagementService);
}
