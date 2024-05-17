void WebContentsImpl::OnDidLoadResourceFromMemoryCache(
    RenderFrameHostImpl* source,
    const GURL& url,
    const std::string& http_method,
    const std::string& mime_type,
    ResourceType resource_type) {
  for (auto& observer : observers_)
    observer.DidLoadResourceFromMemoryCache(url, mime_type, resource_type);

  if (url.is_valid() && url.SchemeIsHTTPOrHTTPS()) {
    StoragePartition* partition = source->GetProcess()->GetStoragePartition();
    scoped_refptr<net::URLRequestContextGetter> request_context(
        resource_type == RESOURCE_TYPE_MEDIA
            ? partition->GetMediaURLRequestContext()
            : partition->GetURLRequestContext());
    BrowserThread::PostTask(
        BrowserThread::IO, FROM_HERE,
        base::Bind(&NotifyCacheOnIO, request_context, url, http_method));
  }
}
