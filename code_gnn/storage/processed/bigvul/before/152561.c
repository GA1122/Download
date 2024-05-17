void RenderFrameImpl::WidgetWillClose() {
  for (auto& observer : observers_)
    observer.WidgetWillClose();
}
