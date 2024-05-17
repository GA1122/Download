void LauncherView::OnMouseMoved(const views::MouseEvent& event) {
  if (ShouldHideTooltip(event.location()) && tooltip_->IsVisible())
    tooltip_->Close();
}
