void RenderFrameImpl::DraggableRegionsChanged() {
  for (auto& observer : observers_)
    observer.DraggableRegionsChanged();
}
