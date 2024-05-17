void ContentSecurityPolicy::UpgradeInsecureRequests() {
  insecure_request_policy_ |= kUpgradeInsecureRequests;
}
