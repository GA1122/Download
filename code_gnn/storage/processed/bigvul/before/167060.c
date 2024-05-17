void ExtensionInstallDialogView::OnNativeThemeChanged(
    const ui::NativeTheme* theme) {
  scroll_view_->SetBackgroundColor(
      theme->GetSystemColor(ui::NativeTheme::kColorId_DialogBackground));
}
