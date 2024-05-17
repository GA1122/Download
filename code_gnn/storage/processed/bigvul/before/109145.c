void RenderViewImpl::queryStorageUsageAndQuota(
    WebFrame* frame,
    WebStorageQuotaType type,
    WebStorageQuotaCallbacks* callbacks) {
  DCHECK(frame);
  WebSecurityOrigin origin = frame->document().securityOrigin();
  if (origin.isUnique()) {
    callbacks->didFail(WebKit::WebStorageQuotaErrorAbort);
    return;
  }
  ChildThread::current()->quota_dispatcher()->QueryStorageUsageAndQuota(
      GURL(origin.toString()),
      static_cast<quota::StorageType>(type),
      QuotaDispatcher::CreateWebStorageQuotaCallbacksWrapper(callbacks));
}
