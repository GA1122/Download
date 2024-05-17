static inline void ClipRectsToImageRect(const FloatRect& image_rect,
                                        FloatRect* src_rect,
                                        FloatRect* dst_rect) {
  if (image_rect.Contains(*src_rect))
    return;

  FloatSize scale(dst_rect->Size().Width() / src_rect->Size().Width(),
                  dst_rect->Size().Height() / src_rect->Size().Height());
  FloatPoint scaled_src_location = src_rect->Location();
  scaled_src_location.Scale(scale.Width(), scale.Height());
  FloatSize offset = dst_rect->Location() - scaled_src_location;

  src_rect->Intersect(image_rect);

  *dst_rect = *src_rect;
  dst_rect->Scale(scale.Width(), scale.Height());
  dst_rect->Move(offset);
}
