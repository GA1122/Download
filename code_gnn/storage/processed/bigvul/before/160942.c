void ChromeClientImpl::ContentsSizeChanged(LocalFrame* frame,
                                           const IntSize& size) const {
  web_view_->DidChangeContentsSize();

  WebLocalFrameImpl* webframe = WebLocalFrameImpl::FromFrame(frame);
  webframe->DidChangeContentsSize(size);
}
