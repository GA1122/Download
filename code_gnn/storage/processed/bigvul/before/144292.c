void LoginDisplayHostWebUI::DisableRestrictiveProxyCheckForTest() {
  default_host()
      ->GetOobeUI()
      ->GetGaiaScreenView()
      ->DisableRestrictiveProxyCheckForTest();
}
