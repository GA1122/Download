void PDFiumEngine::PaintPageShadow(int progressive_index,
                                   pp::ImageData* image_data) {
  DCHECK_GE(progressive_index, 0);
  DCHECK_LT(static_cast<size_t>(progressive_index), progressive_paints_.size());
  DCHECK(image_data);

  int page_index = progressive_paints_[progressive_index].page_index;
  const pp::Rect& dirty_in_screen = progressive_paints_[progressive_index].rect;
  pp::Rect page_rect = pages_[page_index]->rect();
  pp::Rect shadow_rect(page_rect);
  shadow_rect.Inset(-kPageShadowLeft, -kPageShadowTop, -kPageShadowRight,
                    -kPageShadowBottom);

  shadow_rect = GetScreenRect(shadow_rect);
  page_rect = shadow_rect;

  page_rect.Inset(static_cast<int>(ceil(kPageShadowLeft * current_zoom_)),
                  static_cast<int>(ceil(kPageShadowTop * current_zoom_)),
                  static_cast<int>(ceil(kPageShadowRight * current_zoom_)),
                  static_cast<int>(ceil(kPageShadowBottom * current_zoom_)));

  DrawPageShadow(page_rect, shadow_rect, dirty_in_screen, image_data);
}
