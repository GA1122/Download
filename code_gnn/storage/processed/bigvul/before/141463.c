void PaintLayerScrollableArea::PositionOverflowControls() {
  if (!HasScrollbar() && !GetLayoutBox()->CanResize())
    return;

  const IntRect border_box =
      GetLayoutBox()->PixelSnappedBorderBoxRect(layer_->SubpixelAccumulation());

  if (Scrollbar* vertical_scrollbar = VerticalScrollbar())
    vertical_scrollbar->SetFrameRect(RectForVerticalScrollbar(border_box));

  if (Scrollbar* horizontal_scrollbar = HorizontalScrollbar())
    horizontal_scrollbar->SetFrameRect(RectForHorizontalScrollbar(border_box));

  const IntRect& scroll_corner = ScrollCornerRect();
  if (scroll_corner_)
    scroll_corner_->SetFrameRect(LayoutRect(scroll_corner));

  if (resizer_)
    resizer_->SetFrameRect(
        LayoutRect(ResizerCornerRect(border_box, kResizerForPointer)));

  if (Layer()->HasCompositedLayerMapping())
    Layer()->GetCompositedLayerMapping()->PositionOverflowControlsLayers();
}
