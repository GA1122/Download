void WebLocalFrameImpl::DidChangeContentsSize(const IntSize& size) {
  if (text_finder_ && text_finder_->TotalMatchCount() > 0)
    text_finder_->IncreaseMarkerVersion();
}
