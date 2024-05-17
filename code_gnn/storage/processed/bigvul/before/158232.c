void RenderWidgetHostImpl::AddObserver(RenderWidgetHostObserver* observer) {
  observers_.AddObserver(observer);
}
