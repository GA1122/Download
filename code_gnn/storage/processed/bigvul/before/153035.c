pp::Rect PDFiumEngine::GetPageRect(int index) {
  pp::Rect rc(pages_[index]->rect());
  rc.Inset(-kPageShadowLeft, -kPageShadowTop,
           -kPageShadowRight, -kPageShadowBottom);
  return rc;
}
