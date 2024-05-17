void PDFiumEngine::Highlight(void* buffer,
                             int stride,
                             const pp::Rect& rect,
                             std::vector<pp::Rect>* highlighted_rects) {
  if (!buffer)
    return;

  pp::Rect new_rect = rect;
  for (const auto& highlighted : *highlighted_rects)
    new_rect = new_rect.Subtract(highlighted);
  if (new_rect.IsEmpty())
    return;

  std::vector<size_t> overlapping_rect_indices;
  for (size_t i = 0; i < highlighted_rects->size(); ++i) {
    if (new_rect.Intersects((*highlighted_rects)[i]))
      overlapping_rect_indices.push_back(i);
  }

  highlighted_rects->push_back(new_rect);
  int l = new_rect.x();
  int t = new_rect.y();
  int w = new_rect.width();
  int h = new_rect.height();

  for (int y = t; y < t + h; ++y) {
    for (int x = l; x < l + w; ++x) {
      bool overlaps = false;
      for (size_t i : overlapping_rect_indices) {
        const auto& highlighted = (*highlighted_rects)[i];
        if (highlighted.Contains(x, y)) {
          overlaps = true;
          break;
        }
      }
      if (overlaps)
        continue;

      uint8_t* pixel = static_cast<uint8_t*>(buffer) + y * stride + x * 4;
      pixel[0] = static_cast<uint8_t>(pixel[0] * (kHighlightColorB / 255.0));
      pixel[1] = static_cast<uint8_t>(pixel[1] * (kHighlightColorG / 255.0));
      pixel[2] = static_cast<uint8_t>(pixel[2] * (kHighlightColorR / 255.0));
    }
  }
}
