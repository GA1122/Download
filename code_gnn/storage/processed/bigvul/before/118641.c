void UserCloudPolicyManagerChromeOS::EnableWildcardLoginCheck(
    const std::string& username) {
  DCHECK(access_token_.empty());
  wildcard_username_ = username;
}
