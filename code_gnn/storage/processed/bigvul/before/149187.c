void LockScreenMediaControlsView::UpdateOpacity() {
  float progress =
      static_cast<float>(contents_view_->GetBoundsInScreen().bottom()) /
      GetBoundsInScreen().bottom();
  contents_view_->layer()->SetOpacity(progress);
}
