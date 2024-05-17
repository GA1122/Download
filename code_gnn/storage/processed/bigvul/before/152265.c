void RenderFrameImpl::DidCreateDocumentElement() {
  GURL url = frame_->GetDocument().Url();
  if (url.is_valid() && url.spec() != url::kAboutBlankURL) {
    blink::WebFrame* main_frame = render_view_->webview()->MainFrame();
    if (frame_ == main_frame) {
      render_view_->Send(new ViewHostMsg_DocumentAvailableInMainFrame(
          render_view_->GetRoutingID(),
          frame_->GetDocument().IsPluginDocument()));
    }
  }

  for (auto& observer : observers_)
    observer.DidCreateDocumentElement();
}
