void RenderFrameImpl::DidObserveNewCssPropertyUsage(int css_property,
                                                    bool is_animated) {
  for (auto& observer : observers_)
    observer.DidObserveNewCssPropertyUsage(css_property, is_animated);
}
