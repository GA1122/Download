void CardUnmaskPromptViews::OnNativeThemeChanged(const ui::NativeTheme* theme) {
  SkColor bg_color =
      theme->GetSystemColor(ui::NativeTheme::kColorId_DialogBackground);
  progress_overlay_->set_background(
      views::Background::CreateSolidBackground(bg_color));
  progress_label_->SetBackgroundColor(bg_color);
}
