SkColor AutofillPopupBaseView::GetSelectedBackgroundColor() {
  return GetNativeTheme()->GetSystemColor(
      ui::NativeTheme::kColorId_FocusedHighlightedMenuItemBackgroundColor);
}
