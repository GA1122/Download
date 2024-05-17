std::unique_ptr<views::Border> AutofillPopupViewViews::CreateBorder() {
  return views::CreateSolidBorder(
      kPopupBorderThicknessDp,
      GetNativeTheme()->GetSystemColor(
          ui::NativeTheme::kColorId_UnfocusedBorderColor));
}
