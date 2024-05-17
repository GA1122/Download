  void OnCompositorVisible(CompositorImpl* compositor) {
    bool element_inserted = visible_compositors_.insert(compositor).second;
    DCHECK(element_inserted);
    if (visible_compositors_.size() == 1)
      OnVisibilityChanged();
  }
