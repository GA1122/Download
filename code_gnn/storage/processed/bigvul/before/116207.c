void SSLManager::DidStartResourceResponse(ResourceRequestDetails* details) {
  scoped_refptr<SSLRequestInfo> info(new SSLRequestInfo(
      details->url,
      details->resource_type,
      details->origin_child_id,
      details->ssl_cert_id,
      details->ssl_cert_status));

  policy()->OnRequestStarted(info.get());
}
