pp::Rect PDFiumEngine::GetPageScreenRect(int page_index) const {
  return GetScreenRect(pp::Rect(
      0,
      pages_[page_index]->rect().y() - kPageShadowTop,
      document_size_.width(),
      pages_[page_index]->rect().height() + kPageShadowTop +
          kPageShadowBottom + kPageSeparatorThickness));
}
