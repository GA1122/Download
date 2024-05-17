void RenderView::didStartLoading() {
  if (is_loading_) {
    DLOG(WARNING) << "didStartLoading called while loading";
    return;
  }

  is_loading_ = true;
  first_default_plugin_.reset();

  Send(new ViewHostMsg_DidStartLoading(routing_id_));

  FOR_EACH_OBSERVER(RenderViewObserver, observers_, DidStartLoading());
}
