void LockContentsView::LayoutPublicSessionView() {
  gfx::Rect bounds = GetContentsBounds();
  bounds.ClampToCenteredSize(expanded_view_->GetPreferredSize());
  expanded_view_->SetBoundsRect(bounds);
}
