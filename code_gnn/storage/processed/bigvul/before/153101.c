void PDFiumEngine::MouseDownState::Reset() {
  area_ = PDFiumPage::NONSELECTABLE_AREA;
  target_ = PDFiumPage::LinkTarget();
}
