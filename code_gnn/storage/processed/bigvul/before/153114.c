void PDFiumEngine::MouseDownState::Set(const PDFiumPage::Area& area,
                                       const PDFiumPage::LinkTarget& target) {
  area_ = area;
  target_ = target;
}
