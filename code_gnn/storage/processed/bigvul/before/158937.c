void PDFiumEngine::GetAllScreenRectsUnion(std::vector<PDFiumRange>* rect_range,
                                          const pp::Point& offset_point,
                                          std::vector<pp::Rect>* rect_vector) {
  for (auto& range : *rect_range) {
    pp::Rect result_rect;
    const std::vector<pp::Rect>& rects =
        range.GetScreenRects(offset_point, current_zoom_, current_rotation_);
    for (const auto& rect : rects)
      result_rect = result_rect.Union(rect);
    rect_vector->push_back(result_rect);
  }
}
