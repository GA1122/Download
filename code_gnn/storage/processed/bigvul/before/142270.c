void ChromePasswordManagerClient::CheckProtectedPasswordEntry(
    PasswordType reused_password_type,
    const std::vector<std::string>& matching_domains,
    bool password_field_exists) {
  safe_browsing::PasswordProtectionService* pps =
      GetPasswordProtectionService();
  if (!pps)
    return;
  pps->MaybeStartProtectedPasswordEntryRequest(
      web_contents(), GetMainFrameURL(),
      safe_browsing::PasswordProtectionService::
          GetPasswordProtectionReusedPasswordType(reused_password_type),
      matching_domains, password_field_exists);
}
