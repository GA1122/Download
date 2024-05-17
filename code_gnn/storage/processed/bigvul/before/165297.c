ChromeContentBrowserClient::GetOriginPolicyErrorPage(
    content::OriginPolicyErrorReason error_reason,
    const url::Origin& origin,
    const GURL& url) {
  return security_interstitials::OriginPolicyUI::GetErrorPage(error_reason,
                                                              origin, url);
}
