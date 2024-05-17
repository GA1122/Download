void WebContentsImpl::OnDocumentLoadedInFrame() {
  if (!HasValidFrameSource())
    return;

  RenderFrameHostImpl* rfh =
      static_cast<RenderFrameHostImpl*>(render_frame_message_source_);
  FOR_EACH_OBSERVER(
      WebContentsObserver, observers_, DocumentLoadedInFrame(rfh));
}
