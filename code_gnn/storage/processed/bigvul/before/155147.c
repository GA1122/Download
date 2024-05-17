void OmniboxViewViews::OnMouseMoved(const ui::MouseEvent& event) {
  if (location_bar_view_)
    location_bar_view_->OnOmniboxHovered(true);
}
