void EnterpriseEnrollmentScreen::OnOAuthTokenAvailable(
    const std::string& user,
    const std::string& token) {
  user_ = user;
  RegisterForDevicePolicy(token,
                          policy::BrowserPolicyConnector::TOKEN_TYPE_OAUTH);
}
