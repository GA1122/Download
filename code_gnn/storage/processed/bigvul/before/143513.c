  void OnCompositorHidden(CompositorImpl* compositor) {
    size_t elements_removed = visible_compositors_.erase(compositor);
    DCHECK_EQ(1u, elements_removed);
    if (visible_compositors_.size() == 0)
      OnVisibilityChanged();
  }
