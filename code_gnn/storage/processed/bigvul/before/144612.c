void WebContentsImpl::OnPepperInstanceDeleted() {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_, PepperInstanceDeleted());
}
