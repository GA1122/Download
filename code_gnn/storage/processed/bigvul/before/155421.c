void ChromeContentBrowserClient::OnTrustAnchorUsed(
    const std::string& username_hash) {
  policy::PolicyCertServiceFactory::SetUsedPolicyCertificates(username_hash);
}
