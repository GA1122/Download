void WebContentsImpl::OnDocumentLoadedInFrame(int64 frame_id) {
  FOR_EACH_OBSERVER(
      WebContentsObserver, observers_,
      DocumentLoadedInFrame(frame_id, render_view_message_source_));
}
