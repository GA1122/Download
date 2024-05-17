OpenPDFInReaderView::OpenPDFInReaderView() : bubble_(NULL), model_(NULL) {
  SetAccessibilityFocusable(true);
  SetImage(ui::ResourceBundle::GetSharedInstance().GetImageSkiaNamed(
      IDR_OMNIBOX_PDF_ICON));
  SetTooltipText(l10n_util::GetStringUTF16(IDS_PDF_BUBBLE_OPEN_IN_READER_LINK));
}
