void WebLocalFrameImpl::DidChangeContentsSize(const IntSize& size) {
  if (GetTextFinder() && GetTextFinder()->TotalMatchCount() > 0)
    GetTextFinder()->IncreaseMarkerVersion();
}
