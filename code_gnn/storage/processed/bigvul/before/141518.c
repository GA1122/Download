void PaintLayerScrollableArea::UpdateScrollbarEnabledState() {
  bool force_disable =
      GetPageScrollbarTheme().ShouldDisableInvisibleScrollbars() &&
      ScrollbarsHiddenIfOverlay();

  if (HorizontalScrollbar())
    HorizontalScrollbar()->SetEnabled(HasHorizontalOverflow() &&
                                      !force_disable);
  if (VerticalScrollbar())
    VerticalScrollbar()->SetEnabled(HasVerticalOverflow() && !force_disable);
}
