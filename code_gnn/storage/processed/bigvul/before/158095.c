WebURLRequest::PreviewsState LocalFrameClientImpl::GetPreviewsStateForFrame()
    const {
  if (web_frame_->Client())
    return web_frame_->Client()->GetPreviewsStateForFrame();
  return WebURLRequest::kPreviewsUnspecified;
}
