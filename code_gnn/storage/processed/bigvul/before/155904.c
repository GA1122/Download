void ProfileChooserView::OnNativeThemeChanged(
    const ui::NativeTheme* native_theme) {
  views::BubbleDialogDelegateView::OnNativeThemeChanged(native_theme);
  SetBackground(views::CreateSolidBackground(GetNativeTheme()->GetSystemColor(
      ui::NativeTheme::kColorId_DialogBackground)));
}
