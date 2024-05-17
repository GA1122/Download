void PaintLayerScrollableArea::UpdateScrollCornerStyle() {
  if (!scroll_corner_ && !HasScrollbar())
    return;
  if (!scroll_corner_ && HasOverlayScrollbars())
    return;

  const LayoutObject& style_source = ScrollbarStyleSource(*GetLayoutBox());
  scoped_refptr<ComputedStyle> corner =
      GetLayoutBox()->HasOverflowClip()
          ? style_source.GetUncachedPseudoStyle(
                PseudoStyleRequest(kPseudoIdScrollbarCorner),
                style_source.Style())
          : scoped_refptr<ComputedStyle>(nullptr);
  if (corner) {
    if (!scroll_corner_) {
      scroll_corner_ = LayoutScrollbarPart::CreateAnonymous(
          &GetLayoutBox()->GetDocument(), this);
      scroll_corner_->SetDangerousOneWayParent(GetLayoutBox());
    }
    scroll_corner_->SetStyleWithWritingModeOfParent(std::move(corner));
  } else if (scroll_corner_) {
    scroll_corner_->Destroy();
    scroll_corner_ = nullptr;
  }
}
