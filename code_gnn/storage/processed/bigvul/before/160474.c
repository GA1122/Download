void RenderFrameHostImpl::OnStreamHandleConsumed(const GURL& stream_url) {
  if (stream_handle_ && stream_handle_->GetURL() == stream_url)
    stream_handle_.reset();
}
