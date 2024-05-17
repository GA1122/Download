void RenderViewImpl::didStartLoading() {
#if defined(OS_CHROMEOS)
  LOG(ERROR) << "didStartLoading is_loading=" << is_loading_;
#endif
  if (is_loading_) {
    DVLOG(1) << "didStartLoading called while loading";
    return;
  }

  is_loading_ = true;

  Send(new ViewHostMsg_DidStartLoading(routing_id_));

  FOR_EACH_OBSERVER(RenderViewObserver, observers_, DidStartLoading());
}
