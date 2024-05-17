void LockContentsView::SetDisplayStyle(DisplayStyle style) {
  const bool show_expanded_view =
      style == DisplayStyle::kExclusivePublicAccountExpandedView;
  expanded_view_->SetVisible(show_expanded_view);
  main_view_->SetVisible(!show_expanded_view);
  top_header_->SetVisible(!show_expanded_view);
  Layout();
}
