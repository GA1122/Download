void PDFiumEngine::DrawPageShadow(const pp::Rect& page_rc,
                                  const pp::Rect& shadow_rc,
                                  const pp::Rect& clip_rc,
                                  pp::ImageData* image_data) {
  pp::Rect page_rect(page_rc);
  page_rect.Offset(page_offset_);

  pp::Rect shadow_rect(shadow_rc);
  shadow_rect.Offset(page_offset_);

  pp::Rect clip_rect(clip_rc);
  clip_rect.Offset(page_offset_);

  const double factor = 0.5;
  uint32_t depth =
      std::max(std::max(page_rect.x() - shadow_rect.x(),
                        page_rect.y() - shadow_rect.y()),
               std::max(shadow_rect.right() - page_rect.right(),
                        shadow_rect.bottom() - page_rect.bottom()));
  depth = static_cast<uint32_t>(depth * 1.5) + 1;

  if (!page_shadow_.get() || page_shadow_->depth() != depth) {
    page_shadow_ = std::make_unique<ShadowMatrix>(
        depth, factor, client_->GetBackgroundColor());
  }

  DCHECK(!image_data->is_null());
  DrawShadow(image_data, shadow_rect, page_rect, clip_rect, *page_shadow_);
}
