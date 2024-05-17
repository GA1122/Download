void LauncherView::OnMouseExited(const views::MouseEvent& event) {
  gfx::Point point = event.location();
  views::View::ConvertPointToView(parent(), this, &point);
  if (ShouldHideTooltip(point) && tooltip_->IsVisible())
    tooltip_->Close();
}
