void OmniboxViewViews::GetAccessibleState(ui::AccessibleViewState* state) {
  location_bar_view_->GetAccessibleState(state);
  state->role = ui::AccessibilityTypes::ROLE_TEXT;
}
