LayoutSize PaintLayerScrollableArea::MinimumSizeForResizing(float zoom_factor) {
  LayoutUnit min_width =
      MinimumValueForLength(GetLayoutBox()->StyleRef().MinWidth(),
                            GetLayoutBox()->ContainingBlock()->Size().Width());
  LayoutUnit min_height =
      MinimumValueForLength(GetLayoutBox()->StyleRef().MinHeight(),
                            GetLayoutBox()->ContainingBlock()->Size().Height());
  min_width = std::max(LayoutUnit(min_width / zoom_factor),
                       LayoutUnit(kDefaultMinimumWidthForResizing));
  min_height = std::max(LayoutUnit(min_height / zoom_factor),
                        LayoutUnit(kDefaultMinimumHeightForResizing));
  return LayoutSize(min_width, min_height);
}
