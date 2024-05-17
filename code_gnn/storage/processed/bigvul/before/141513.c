void PaintLayerScrollableArea::UpdateResizerStyle(
    const ComputedStyle* old_style) {
  if (!RuntimeEnabledFeatures::CompositeAfterPaintEnabled() && old_style &&
      old_style->Resize() != GetLayoutBox()->StyleRef().Resize()) {
    if (auto* graphics_layer = LayerForScrollCorner())
      graphics_layer->SetNeedsDisplay();
  }

  if (!resizer_ && !GetLayoutBox()->CanResize())
    return;

  const LayoutObject& style_source = ScrollbarStyleSource(*GetLayoutBox());
  scoped_refptr<ComputedStyle> resizer =
      GetLayoutBox()->HasOverflowClip()
          ? style_source.GetUncachedPseudoStyle(
                PseudoStyleRequest(kPseudoIdResizer), style_source.Style())
          : scoped_refptr<ComputedStyle>(nullptr);
  if (resizer) {
    if (!resizer_) {
      resizer_ = LayoutScrollbarPart::CreateAnonymous(
          &GetLayoutBox()->GetDocument(), this);
      resizer_->SetDangerousOneWayParent(GetLayoutBox());
    }
    resizer_->SetStyleWithWritingModeOfParent(std::move(resizer));
  } else if (resizer_) {
    resizer_->Destroy();
    resizer_ = nullptr;
  }
}
