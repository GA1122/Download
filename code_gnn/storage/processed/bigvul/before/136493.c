static FloatClipRect GetClipRect(const ClipPaintPropertyNode* clip_node,
                                 OverlayScrollbarClipBehavior clip_behavior) {
  FloatClipRect clip_rect(
      UNLIKELY(clip_behavior == kExcludeOverlayScrollbarSizeForHitTesting)
          ? clip_node->ClipRectExcludingOverlayScrollbars()
          : clip_node->ClipRect());
  if (clip_node->ClipPath())
    clip_rect.ClearIsTight();
  return clip_rect;
}
