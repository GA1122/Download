bool GetEnterpriseDomain(std::string* out_domain) {
  policy::BrowserPolicyConnectorChromeOS* policy_connector =
      g_browser_process->platform_part()->browser_policy_connector_chromeos();
  if (policy_connector->IsCloudManaged()) {
    *out_domain = policy_connector->GetEnterpriseDisplayDomain();
    return true;
  }
  return false;
}
