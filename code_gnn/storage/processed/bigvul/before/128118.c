void CastDuplexView::ActivateCastView() {
  select_view_->SetVisible(false);
  cast_view_->SetVisible(true);
  InvalidateLayout();
}
