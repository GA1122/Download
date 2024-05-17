static CompositingReasons CompositingReasonsForTransform(const LayoutBox& box) {
  if (!box.HasLayer())
    return CompositingReason::kNone;

  const ComputedStyle& style = box.StyleRef();
  CompositingReasons compositing_reasons = CompositingReason::kNone;
  if (CompositingReasonFinder::RequiresCompositingForTransform(box))
    compositing_reasons |= CompositingReason::k3DTransform;

  if (CompositingReasonFinder::RequiresCompositingForTransformAnimation(style))
    compositing_reasons |= CompositingReason::kActiveTransformAnimation;

  if (style.HasWillChangeCompositingHint() &&
      !style.SubtreeWillChangeContents())
    compositing_reasons |= CompositingReason::kWillChangeCompositingHint;

  if (box.HasLayer() && box.Layer()->Has3DTransformedDescendant()) {
    if (style.HasPerspective())
      compositing_reasons |= CompositingReason::kPerspectiveWith3DDescendants;
    if (style.UsedTransformStyle3D() == ETransformStyle3D::kPreserve3d)
      compositing_reasons |= CompositingReason::kPreserve3DWith3DDescendants;
  }

  return compositing_reasons;
}
