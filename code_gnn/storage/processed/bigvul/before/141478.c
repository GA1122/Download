IntRect PaintLayerScrollableArea::ScrollCornerAndResizerRect() const {
  IntRect scroll_corner_and_resizer = ScrollCornerRect();
  if (scroll_corner_and_resizer.IsEmpty()) {
    scroll_corner_and_resizer =
        ResizerCornerRect(GetLayoutBox()->PixelSnappedBorderBoxRect(
                              Layer()->SubpixelAccumulation()),
                          kResizerForPointer);
  }
  return scroll_corner_and_resizer;
}
