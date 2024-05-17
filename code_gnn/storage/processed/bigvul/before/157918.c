void RenderViewImpl::DidUpdateMainFrameLayout() {
  for (auto& observer : observers_)
    observer.DidUpdateMainFrameLayout();

   needs_preferred_size_update_ = true;
 }
