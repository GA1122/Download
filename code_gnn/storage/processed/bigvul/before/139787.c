void OpenPDFInReaderView::GetAccessibleState(ui::AXViewState* state) {
  state->name = l10n_util::GetStringUTF16(IDS_ACCNAME_OPEN_PDF_IN_READER);
  state->role = ui::AX_ROLE_BUTTON;
}
