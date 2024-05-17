void OmniboxViewViews::OnNativeThemeChanged(const ui::NativeTheme* theme) {
  views::Textfield::OnNativeThemeChanged(theme);
  if (location_bar_view_) {
    SetBackgroundColor(
        location_bar_view_->GetColor(LocationBarView::BACKGROUND));
  }
  EmphasizeURLComponents();
}
