void RenderFrameImpl::requestStorageQuota(
    blink::WebLocalFrame* frame,
    blink::WebStorageQuotaType type,
    unsigned long long requested_size,
    blink::WebStorageQuotaCallbacks callbacks) {
  DCHECK(!frame_ || frame_ == frame);
  WebSecurityOrigin origin = frame->document().securityOrigin();
  if (origin.isUnique()) {
    callbacks.didFail(blink::WebStorageQuotaErrorAbort);
    return;
  }
  ChildThreadImpl::current()->quota_dispatcher()->RequestStorageQuota(
      render_view_->GetRoutingID(),
      GURL(origin.toString()),
      static_cast<storage::StorageType>(type),
      requested_size,
      QuotaDispatcher::CreateWebStorageQuotaCallbacksWrapper(callbacks));
}
