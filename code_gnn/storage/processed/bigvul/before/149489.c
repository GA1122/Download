void ContentSecurityPolicy::upgradeInsecureRequests() {
  m_insecureRequestPolicy |= kUpgradeInsecureRequests;
}
