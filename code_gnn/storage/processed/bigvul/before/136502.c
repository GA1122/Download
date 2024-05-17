  const FloatClipRect* GetCachedClip(
      const ClipPaintPropertyNode* descendant_clip,
      const PropertyTreeState& ancestor_property_tree_state) {
    GeometryMapperClipCache::ClipAndTransform clip_and_transform(
        ancestor_property_tree_state.Clip(),
        ancestor_property_tree_state.Transform(),
        kIgnorePlatformOverlayScrollbarSize);
    return descendant_clip->GetClipCache().GetCachedClip(clip_and_transform);
  }
