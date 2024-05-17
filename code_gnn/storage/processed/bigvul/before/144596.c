void WebContentsImpl::OnDidDisplayContentWithCertificateErrors(
    const GURL& url,
    const std::string& security_info) {
  SSLStatus ssl;
  if (!DeserializeSecurityInfo(security_info, &ssl)) {
    bad_message::ReceivedBadMessage(
        GetRenderProcessHost(),
        bad_message::WC_CONTENT_WITH_CERT_ERRORS_BAD_SECURITY_INFO);
    return;
  }

  displayed_insecure_content_ = true;
  SSLManager::NotifySSLInternalStateChanged(
      GetController().GetBrowserContext());
}
