 void AwFeatureListCreator::CreateLocalState() {
  browser_policy_connector_ = std::make_unique<AwBrowserPolicyConnector>();
  local_state_ = CreatePrefService();
}
