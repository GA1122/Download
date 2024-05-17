  void LocalToAncestorVisualRectInternal(
      const PropertyTreeState& local_state,
      const PropertyTreeState& ancestor_state,
      FloatClipRect& mapping_rect,
      bool& success) {
    GeometryMapper::LocalToAncestorVisualRectInternal(
        local_state, ancestor_state, mapping_rect,
        kIgnorePlatformOverlayScrollbarSize, kNonInclusiveIntersect, success);
  }
