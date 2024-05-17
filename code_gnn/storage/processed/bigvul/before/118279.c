void AutofillDialogViews::OverlayView::OnNativeThemeChanged(
    const ui::NativeTheme* theme) {
  set_background(views::Background::CreateSolidBackground(
      theme->GetSystemColor(ui::NativeTheme::kColorId_DialogBackground)));
}
