void WebContentsImpl::OnDidLoadResourceFromMemoryCache(
    const GURL& url,
    const std::string& security_info,
    const std::string& http_method,
    const std::string& mime_type,
    ResourceType resource_type) {
  SSLStatus status;
  if (!DeserializeSecurityInfo(security_info, &status)) {
    bad_message::ReceivedBadMessage(
        GetRenderProcessHost(),
        bad_message::WC_MEMORY_CACHE_RESOURCE_BAD_SECURITY_INFO);
    return;
  }

  LoadFromMemoryCacheDetails details(
      url, status.cert_id, status.cert_status, http_method, mime_type,
      resource_type);

  controller_.ssl_manager()->DidLoadFromMemoryCache(details);

  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidLoadResourceFromMemoryCache(details));

  if (url.is_valid() && url.SchemeIsHTTPOrHTTPS()) {
    scoped_refptr<net::URLRequestContextGetter> request_context(
        resource_type == RESOURCE_TYPE_MEDIA ?
            GetBrowserContext()->GetMediaRequestContextForRenderProcess(
                GetRenderProcessHost()->GetID()) :
            GetBrowserContext()->GetRequestContextForRenderProcess(
                GetRenderProcessHost()->GetID()));
    BrowserThread::PostTask(
        BrowserThread::IO,
        FROM_HERE,
        base::Bind(&NotifyCacheOnIO, request_context, url, http_method));
  }
}
