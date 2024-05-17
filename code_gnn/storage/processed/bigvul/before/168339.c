void PaintDetachedBookmarkBar(gfx::Canvas* canvas,
                              BookmarkBarView* view) {
  const ui::ThemeProvider* tp = view->GetThemeProvider();
  gfx::Rect fill_rect = view->GetLocalBounds();

  canvas->FillRect(fill_rect,
                   tp->GetColor(ThemeProperties::COLOR_CONTROL_BACKGROUND));
  canvas->FillRect(
      fill_rect,
      tp->GetColor(ThemeProperties::COLOR_DETACHED_BOOKMARK_BAR_BACKGROUND));

  BrowserView::Paint1pxHorizontalLine(
      canvas,
      tp->GetColor(ThemeProperties::COLOR_DETACHED_BOOKMARK_BAR_SEPARATOR),
      view->GetLocalBounds(), true);
}
