bool ChromeContentBrowserClient::PreSpawnRenderer(
    sandbox::TargetPolicy* policy) {
  sandbox::ResultCode result = policy->AddRule(
      sandbox::TargetPolicy::SUBSYS_NAMED_PIPES,
      sandbox::TargetPolicy::NAMEDPIPES_ALLOW_ANY,
      L"\\\\.\\pipe\\chrome.nacl.*");
  if (result != sandbox::SBOX_ALL_OK)
    return false;
  return result == sandbox::SBOX_ALL_OK;
}
