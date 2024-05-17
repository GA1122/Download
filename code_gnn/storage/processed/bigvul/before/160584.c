void RenderFrameImpl::DidFailProvisionalLoad(
    const WebURLError& error,
    blink::WebHistoryCommitType commit_type) {
  DidFailProvisionalLoadInternal(error, commit_type, base::nullopt);
}
