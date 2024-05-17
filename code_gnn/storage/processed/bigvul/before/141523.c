IntRect PaintLayerScrollableArea::VisibleContentRect(
    IncludeScrollbarsInRect scrollbar_inclusion) const {
  LayoutRect layout_content_rect(LayoutContentRect(scrollbar_inclusion));
  return IntRect(FlooredIntPoint(layout_content_rect.Location()),
                 PixelSnappedIntSize(layout_content_rect.Size(),
                                     GetLayoutBox()->Location()));
}
