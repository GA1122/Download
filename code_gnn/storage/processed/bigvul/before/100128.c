void BrowserActionsContainer::Paint(gfx::Canvas* canvas) {
  DetachableToolbarView::PaintVerticalDivider(
      canvas,
      width() - kDividerHorizontalMargin, height(), kDividerVerticalPadding,
      DetachableToolbarView::kEdgeDividerColor,
      DetachableToolbarView::kMiddleDividerColor,
      GetThemeProvider()->GetColor(BrowserThemeProvider::COLOR_TOOLBAR));
}
