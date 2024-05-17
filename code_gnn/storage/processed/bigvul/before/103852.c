void RenderView::RemoveObserver(RenderViewObserver* observer) {
  observer->set_render_view(NULL);
  observers_.RemoveObserver(observer);
}
