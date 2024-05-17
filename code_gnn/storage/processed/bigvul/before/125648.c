void RenderViewHostImpl::OnDidChangeScrollbarsForMainFrame(
    bool has_horizontal_scrollbar, bool has_vertical_scrollbar) {
  if (view_)
    view_->SetHasHorizontalScrollbar(has_horizontal_scrollbar);
}
