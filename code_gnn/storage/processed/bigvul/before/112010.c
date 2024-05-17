void SyncTest::SetupMockGaiaResponses() {
  username_ = "user@gmail.com";
  password_ = "password";
  factory_.reset(new URLFetcherImplFactory());
  fake_factory_.reset(new FakeURLFetcherFactory(factory_.get()));
  fake_factory_->SetFakeResponse(
      GaiaUrls::GetInstance()->client_login_url(),
      "SID=sid\nLSID=lsid",
      true);
  fake_factory_->SetFakeResponse(
      GaiaUrls::GetInstance()->get_user_info_url(),
      "email=user@gmail.com",
      true);
  fake_factory_->SetFakeResponse(
      GaiaUrls::GetInstance()->issue_auth_token_url(),
      "auth",
      true);
  fake_factory_->SetFakeResponse(
      GoogleURLTracker::kSearchDomainCheckURL,
      ".google.com",
      true);
  fake_factory_->SetFakeResponse(
      GaiaUrls::GetInstance()->client_login_to_oauth2_url(),
      "some_response",
      true);
  fake_factory_->SetFakeResponse(
      GaiaUrls::GetInstance()->oauth2_token_url(),
      "{"
      "  \"refresh_token\": \"rt1\","
      "  \"access_token\": \"at1\","
      "  \"expires_in\": 3600,"
      "  \"token_type\": \"Bearer\""
      "}",
      true);
}
