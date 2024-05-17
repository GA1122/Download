void PaintAttachedBookmarkBar(gfx::Canvas* canvas,
                              BookmarkBarView* view,
                              BrowserView* browser_view,
                              int toolbar_overlap) {
  gfx::Point background_image_offset =
      browser_view->OffsetPointForToolbarBackgroundImage(
          gfx::Point(view->GetMirroredX(), view->y()));
  PaintBackgroundAttachedMode(canvas, view->GetThemeProvider(),
                              view->GetLocalBounds(), background_image_offset);
  if (view->height() >= toolbar_overlap) {
    BrowserView::Paint1pxHorizontalLine(
        canvas, view->GetThemeProvider()->GetColor(
                    ThemeProperties::COLOR_TOOLBAR_BOTTOM_SEPARATOR),
        view->GetLocalBounds(), true);
  }
}
