void WebContentsImpl::DidGetResourceResponseStart(
  const ResourceRequestDetails& details) {
  controller_.ssl_manager()->DidStartResourceResponse(
      details.url, details.has_certificate, details.ssl_cert_status);

  for (auto& observer : observers_)
    observer.DidGetResourceResponseStart(details);
}
