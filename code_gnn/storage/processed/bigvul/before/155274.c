void ChromeContentBrowserClient::AllowCertificateError(
    content::WebContents* web_contents,
    int cert_error,
    const net::SSLInfo& ssl_info,
    const GURL& request_url,
    ResourceType resource_type,
    bool strict_enforcement,
    bool expired_previous_decision,
    const base::Callback<void(content::CertificateRequestResultType)>&
        callback) {
  DCHECK(web_contents);
  if (resource_type != content::RESOURCE_TYPE_MAIN_FRAME) {
    if (!callback.is_null())
      callback.Run(content::CERTIFICATE_REQUEST_RESULT_TYPE_DENY);
    return;
  }

  prerender::PrerenderContents* prerender_contents =
      prerender::PrerenderContents::FromWebContents(web_contents);
  if (prerender_contents) {
    prerender_contents->Destroy(prerender::FINAL_STATUS_SSL_ERROR);
    if (!callback.is_null()) {
      callback.Run(content::CERTIFICATE_REQUEST_RESULT_TYPE_CANCEL);
    }
    return;
  }

  if (base::FeatureList::IsEnabled(features::kSSLCommittedInterstitials)) {
    callback.Run(content::CERTIFICATE_REQUEST_RESULT_TYPE_DENY);
    return;
  }

  SSLErrorHandler::HandleSSLError(
      web_contents, cert_error, ssl_info, request_url,
      expired_previous_decision,
      std::make_unique<CertificateReportingServiceCertReporter>(web_contents),
      callback, SSLErrorHandler::BlockingPageReadyCallback());
}
