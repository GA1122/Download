void PDFiumEngine::FillPageSides(int progressive_index) {
  DCHECK_GE(progressive_index, 0);
  DCHECK_LT(static_cast<size_t>(progressive_index), progressive_paints_.size());

  int page_index = progressive_paints_[progressive_index].page_index;
  pp::Rect dirty_in_screen = progressive_paints_[progressive_index].rect;
  FPDF_BITMAP bitmap = progressive_paints_[progressive_index].bitmap;

  pp::Rect page_rect = pages_[page_index]->rect();
  if (page_rect.x() > 0) {
    pp::Rect left(0,
                  page_rect.y() - kPageShadowTop,
                  page_rect.x() - kPageShadowLeft,
                  page_rect.height() + kPageShadowTop +
                      kPageShadowBottom + kPageSeparatorThickness);
    left = GetScreenRect(left).Intersect(dirty_in_screen);

    FPDFBitmap_FillRect(bitmap, left.x() - dirty_in_screen.x(),
                        left.y() - dirty_in_screen.y(), left.width(),
                        left.height(), client_->GetBackgroundColor());
  }

  if (page_rect.right() < document_size_.width()) {
    pp::Rect right(page_rect.right() + kPageShadowRight,
                   page_rect.y() - kPageShadowTop,
                   document_size_.width() - page_rect.right() -
                      kPageShadowRight,
                   page_rect.height() + kPageShadowTop +
                       kPageShadowBottom + kPageSeparatorThickness);
    right = GetScreenRect(right).Intersect(dirty_in_screen);

    FPDFBitmap_FillRect(bitmap, right.x() - dirty_in_screen.x(),
                        right.y() - dirty_in_screen.y(), right.width(),
                        right.height(), client_->GetBackgroundColor());
  }

  pp::Rect bottom(page_rect.x() - kPageShadowLeft,
                  page_rect.bottom() + kPageShadowBottom,
                  page_rect.width() + kPageShadowLeft + kPageShadowRight,
                  kPageSeparatorThickness);
  bottom = GetScreenRect(bottom).Intersect(dirty_in_screen);

  FPDFBitmap_FillRect(bitmap, bottom.x() - dirty_in_screen.x(),
                      bottom.y() - dirty_in_screen.y(), bottom.width(),
                      bottom.height(), client_->GetBackgroundColor());
}
