ChromeContentBrowserClient::GetOriginPolicyErrorPage(
    content::OriginPolicyErrorReason error_reason,
    content::NavigationHandle* handle) {
  return security_interstitials::OriginPolicyUI::GetErrorPageAsHTML(
      error_reason, handle);
}
