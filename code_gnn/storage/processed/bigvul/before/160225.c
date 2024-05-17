gfx::PointF PDFiumEngine::TransformPagePoint(int page_index,
                                             const gfx::PointF& page_xy) {
  DCHECK(PageIndexInBounds(page_index));
  return pages_[page_index]->TransformPageToScreenXY(page_xy);
}
