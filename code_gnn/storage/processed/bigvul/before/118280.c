void AutofillDialogViews::OnNativeThemeChanged(
    const ui::NativeTheme* theme) {
  if (!legal_document_view_)
    return;

  views::StyledLabel::RangeStyleInfo default_style;
  default_style.color =
      theme->GetSystemColor(ui::NativeTheme::kColorId_LabelDisabledColor);

  legal_document_view_->SetDefaultStyle(default_style);
}
