void RenderFrameImpl::DidCompleteResponse(
    int request_id,
    const network::URLLoaderCompletionStatus& status) {
  for (auto& observer : observers_)
    observer.DidCompleteResponse(request_id, status);
}
