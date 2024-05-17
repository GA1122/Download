void RenderView::didCreateDocumentElement(WebFrame* frame) {
  GURL url = frame->document().url();
  if (url.is_valid() && url.spec() != chrome::kAboutBlankURL) {
    if (frame == webview()->mainFrame())
      Send(new ViewHostMsg_DocumentAvailableInMainFrame(routing_id_));
  }

  FOR_EACH_OBSERVER(RenderViewObserver, observers_,
                    DidCreateDocumentElement(frame));
}
