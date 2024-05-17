void RenderView::didCompleteClientRedirect(
    WebFrame* frame, const WebURL& from) {
  if (!frame->parent())
    completed_client_redirect_src_ = from;
  FOR_EACH_OBSERVER(
      RenderViewObserver, observers_, DidCompleteClientRedirect(frame, from));
}
