void RenderFrameImpl::didCreateDocumentElement(blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);

  GURL url = frame->document().url();
  if (url.is_valid() && url.spec() != url::kAboutBlankURL) {
    blink::WebFrame* main_frame = render_view_->webview()->mainFrame();
    if (frame == main_frame) {
      render_view_->Send(new ViewHostMsg_DocumentAvailableInMainFrame(
          render_view_->GetRoutingID(),
          main_frame->document().isPluginDocument()));
    }
  }

  FOR_EACH_OBSERVER(RenderFrameObserver, observers_,
                    DidCreateDocumentElement());
  FOR_EACH_OBSERVER(RenderViewObserver, render_view_->observers(),
                    DidCreateDocumentElement(frame));
}
