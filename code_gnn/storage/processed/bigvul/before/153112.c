bool PDFiumEngine::SelectFindResult(bool forward) {
  if (find_results_.empty()) {
    NOTREACHED();
    return false;
  }

  SelectionChangeInvalidator selection_invalidator(this);

  size_t new_index;
  const size_t last_index = find_results_.size() - 1;
  if (current_find_index_.valid()) {
    size_t current_index = current_find_index_.GetIndex();
    if (forward) {
      new_index = (current_index >= last_index) ?  0 : current_index + 1;
    } else {
      new_index = (current_find_index_.GetIndex() == 0) ?
          last_index : current_index - 1;
    }
  } else {
    new_index = forward ? 0 : last_index;
  }
  current_find_index_.SetIndex(new_index);

  selection_.clear();
  selection_.push_back(find_results_[current_find_index_.GetIndex()]);

  pp::Rect bounding_rect;
  pp::Rect visible_rect = GetVisibleRect();
  std::vector<pp::Rect> rects;
  rects = find_results_[current_find_index_.GetIndex()].GetScreenRects(
      pp::Point(), 1.0, current_rotation_);
  for (const auto& rect : rects)
    bounding_rect = bounding_rect.Union(rect);
  if (!visible_rect.Contains(bounding_rect)) {
    pp::Point center = bounding_rect.CenterPoint();
    int new_y = static_cast<int>(center.y() * current_zoom_) -
        static_cast<int>(visible_rect.height() * current_zoom_ / 2);
    if (new_y < 0)
      new_y = 0;
    client_->ScrollToY(new_y);

    if (center.x() < visible_rect.x() || center.x() > visible_rect.right()) {
      int new_x = static_cast<int>(center.x()  * current_zoom_) -
          static_cast<int>(visible_rect.width() * current_zoom_ / 2);
      if (new_x < 0)
        new_x = 0;
      client_->ScrollToX(new_x);
    }
  }

  client_->NotifySelectedFindResultChanged(current_find_index_.GetIndex());
  return true;
}
