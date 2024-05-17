int PaintLayerScrollableArea::VerticalScrollbarStart(int min_x,
                                                     int max_x) const {
  if (GetLayoutBox()->ShouldPlaceBlockDirectionScrollbarOnLogicalLeft())
    return min_x + GetLayoutBox()->BorderLeft().ToInt();
  return max_x - GetLayoutBox()->BorderRight().ToInt() -
         VerticalScrollbar()->ScrollbarThickness();
}
