void CastDuplexView::ActivateSelectView() {
  select_view_->SetVisible(true);
  cast_view_->SetVisible(false);
  InvalidateLayout();
}
