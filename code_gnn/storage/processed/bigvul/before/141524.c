LayoutRect PaintLayerScrollableArea::VisibleScrollSnapportRect(
    IncludeScrollbarsInRect scrollbar_inclusion) const {
  const ComputedStyle* style = GetLayoutBox()->Style();
  LayoutRect layout_content_rect(LayoutContentRect(scrollbar_inclusion));
  layout_content_rect.MoveBy(LayoutPoint(-ScrollOrigin()));
  LayoutRectOutsets padding(MinimumValueForLength(style->ScrollPaddingTop(),
                                                  layout_content_rect.Height()),
                            MinimumValueForLength(style->ScrollPaddingRight(),
                                                  layout_content_rect.Width()),
                            MinimumValueForLength(style->ScrollPaddingBottom(),
                                                  layout_content_rect.Height()),
                            MinimumValueForLength(style->ScrollPaddingLeft(),
                                                  layout_content_rect.Width()));
  layout_content_rect.Contract(padding);
  return layout_content_rect;
}
