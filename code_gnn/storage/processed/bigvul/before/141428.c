int PaintLayerScrollableArea::HypotheticalScrollbarThickness(
    ScrollbarOrientation orientation) const {
  Scrollbar* scrollbar = orientation == kHorizontalScrollbar
                             ? HorizontalScrollbar()
                             : VerticalScrollbar();
  if (scrollbar)
    return scrollbar->ScrollbarThickness();

  const LayoutObject& style_source = ScrollbarStyleSource(*GetLayoutBox());
  bool has_custom_scrollbar_style =
      style_source.StyleRef().HasPseudoStyle(kPseudoIdScrollbar);
  if (has_custom_scrollbar_style) {
    return LayoutScrollbar::HypotheticalScrollbarThickness(
        orientation, *GetLayoutBox(), style_source);
  }

  ScrollbarControlSize scrollbar_size = kRegularScrollbar;
  if (style_source.StyleRef().HasAppearance()) {
    scrollbar_size = LayoutTheme::GetTheme().ScrollbarControlSizeForPart(
        style_source.StyleRef().Appearance());
  }
  ScrollbarTheme& theme = GetPageScrollbarTheme();
  if (theme.UsesOverlayScrollbars())
    return 0;
  int thickness = theme.ScrollbarThickness(scrollbar_size);
  return GetLayoutBox()
      ->GetDocument()
      .GetPage()
      ->GetChromeClient()
      .WindowToViewportScalar(thickness);
}
