void RenderFrameHostImpl::IssueKeepAliveHandle(
    mojom::KeepAliveHandleRequest request) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (!base::FeatureList::IsEnabled(
          features::kKeepAliveRendererForKeepaliveRequests)) {
    bad_message::ReceivedBadMessage(
        GetProcess(), bad_message::RFH_KEEP_ALIVE_HANDLE_REQUESTED_INCORRECTLY);
    return;
  }
  if (GetProcess()->IsKeepAliveRefCountDisabled())
    return;

  if (!keep_alive_handle_factory_) {
    keep_alive_handle_factory_ =
        std::make_unique<KeepAliveHandleFactory>(GetProcess());
    keep_alive_handle_factory_->SetTimeout(keep_alive_timeout_);
  }
  keep_alive_handle_factory_->Create(std::move(request));
}
