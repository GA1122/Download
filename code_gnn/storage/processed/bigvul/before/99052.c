gfx::Size PageInfoWindowView::GetPreferredSize() {
  return gfx::Size(views::Window::GetLocalizedContentsSize(
      IDS_PAGEINFO_DIALOG_WIDTH_CHARS, IDS_PAGEINFO_DIALOG_HEIGHT_LINES));
}
