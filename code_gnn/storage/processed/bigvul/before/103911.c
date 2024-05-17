void RenderView::queryStorageUsageAndQuota(
    WebFrame* frame,
    WebStorageQuotaType type,
    WebStorageQuotaCallbacks* callbacks) {
  DCHECK(frame);
  WebSecurityOrigin origin = frame->document().securityOrigin();
  if (origin.isEmpty()) {
    callbacks->didFail(WebKit::WebStorageQuotaErrorAbort);
    return;
  }
  ChildThread::current()->quota_dispatcher()->QueryStorageUsageAndQuota(
      GURL(origin.toString()), type, callbacks);
}
