gfx::Size ExternalProtocolDialog::CalculatePreferredSize() const {
  constexpr int kDialogContentWidth = 400;
  return gfx::Size(kDialogContentWidth, GetHeightForWidth(kDialogContentWidth));
}
