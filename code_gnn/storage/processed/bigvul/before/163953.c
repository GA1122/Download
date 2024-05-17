void PaymentHandlerWebFlowViewController::VisibleSecurityStateChanged(
    content::WebContents* source) {
  DCHECK(source == web_contents());
  if (!SslValidityChecker::IsSslCertificateValid(source) &&
      !net::IsLocalhost(source->GetLastCommittedURL())) {
    log_.Error("Aborting payment handler window \"" + target_.spec() +
               "\" because of insecure certificate state on \"" +
               source->GetVisibleURL().spec() + "\"");
    AbortPayment();
  }
}
