void SVGElement::EnsureAttributeAnimValUpdated() {
  if (!RuntimeEnabledFeatures::WebAnimationsSVGEnabled())
    return;

  if ((HasSVGRareData() && SvgRareData()->WebAnimatedAttributesDirty()) ||
      (GetElementAnimations() &&
       DocumentAnimations::NeedsAnimationTimingUpdate(GetDocument()))) {
    DocumentAnimations::UpdateAnimationTimingIfNeeded(GetDocument());
    ApplyActiveWebAnimations();
  }
}
