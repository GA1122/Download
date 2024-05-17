float GetItemScale(const gfx::Rect& source,
                   const gfx::Rect& target,
                   int top_view_inset,
                   int title_height) {
  return ScopedTransformOverviewWindow::GetItemScale(
      source.size(), target.size(), top_view_inset, title_height);
}
