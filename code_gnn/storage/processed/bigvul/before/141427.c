int PaintLayerScrollableArea::HorizontalScrollbarStart(int min_x) const {
  int x = min_x + GetLayoutBox()->BorderLeft().ToInt();
  if (GetLayoutBox()->ShouldPlaceBlockDirectionScrollbarOnLogicalLeft())
    x += HasVerticalScrollbar()
             ? VerticalScrollbar()->ScrollbarThickness()
             : ResizerCornerRect(GetLayoutBox()->PixelSnappedBorderBoxRect(
                                     Layer()->SubpixelAccumulation()),
                                 kResizerForPointer)
                   .Width();
  return x;
}
