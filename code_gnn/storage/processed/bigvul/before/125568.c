void RenderViewHostImpl::AddObserver(RenderViewHostObserver* observer) {
  observers_.AddObserver(observer);
}
