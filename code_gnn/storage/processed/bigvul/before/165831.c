void SVGElement::SetWebAnimationsPending() {
  GetDocument().AccessSVGExtensions().AddWebAnimationsPendingSVGElement(*this);
  EnsureSVGRareData()->SetWebAnimatedAttributesDirty(true);
  EnsureUniqueElementData().animated_svg_attributes_are_dirty_ = true;
}
