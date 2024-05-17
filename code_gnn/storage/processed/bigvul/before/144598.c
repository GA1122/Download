void WebContentsImpl::OnDidRunContentWithCertificateErrors(
    const GURL& security_origin,
    const GURL& url,
    const std::string& security_info) {
  SSLStatus ssl;
  if (!DeserializeSecurityInfo(security_info, &ssl)) {
    bad_message::ReceivedBadMessage(
        GetRenderProcessHost(),
        bad_message::WC_CONTENT_WITH_CERT_ERRORS_BAD_SECURITY_INFO);
    return;
  }

  controller_.ssl_manager()->DidRunInsecureContent(security_origin);
  SSLManager::NotifySSLInternalStateChanged(
      GetController().GetBrowserContext());
}
