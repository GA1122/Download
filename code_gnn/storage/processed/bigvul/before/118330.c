void AutofillDialogViews::OverlayView::UpdateState() {
  const DialogOverlayState& state = delegate_->GetDialogOverlay();

  if (state.image.IsEmpty()) {
    SetVisible(false);
    return;
  }

  image_view_->SetImage(state.image.ToImageSkia());

  message_view_->SetVisible(!state.string.text.empty());
  message_view_->SetText(state.string.text);
  message_view_->SetFontList(state.string.font_list);
  message_view_->SetEnabledColor(GetNativeTheme()->GetSystemColor(
      ui::NativeTheme::kColorId_TextfieldReadOnlyColor));

  message_view_->SetBorder(
      views::Border::CreateEmptyBorder(kOverlayMessageVerticalPadding,
                                       kDialogEdgePadding,
                                       kOverlayMessageVerticalPadding,
                                       kDialogEdgePadding));

  SetVisible(true);
}
