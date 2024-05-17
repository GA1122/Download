void PanelBrowserView::OnWindowEndUserBoundsChange() {
  panel_->OnPanelEndUserResizing();

  gfx::Rect new_bounds = GetBounds();
  if (bounds_ == new_bounds)
    return;
  bounds_ = new_bounds;

  panel_->IncreaseMaxSize(bounds_.size());
  panel_->set_full_size(bounds_.size());

  panel_->panel_strip()->RefreshLayout();
}
