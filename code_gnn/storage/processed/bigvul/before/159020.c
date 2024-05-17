bool PDFiumEngine::SelectFindResult(bool forward) {
  if (find_results_.empty()) {
    NOTREACHED();
    return false;
  }

  SelectionChangeInvalidator selection_invalidator(this);

  size_t new_index;
  const size_t last_index = find_results_.size() - 1;

  if (resume_find_index_.valid()) {
    new_index = resume_find_index_.GetIndex();
    resume_find_index_.Invalidate();
  } else if (current_find_index_.valid()) {
    size_t current_index = current_find_index_.GetIndex();
    if ((forward && current_index >= last_index) ||
        (!forward && current_index == 0)) {
      current_find_index_.Invalidate();
      client_->NotifySelectedFindResultChanged(-1);
      client_->NotifyNumberOfFindResultsChanged(find_results_.size(), true);
      return true;
    }
    int increment = forward ? 1 : -1;
    new_index = current_index + increment;
  } else {
    new_index = forward ? 0 : last_index;
  }
  current_find_index_.SetIndex(new_index);

  selection_.clear();
  selection_.push_back(find_results_[current_find_index_.GetIndex()]);

  pp::Rect bounding_rect;
  pp::Rect visible_rect = GetVisibleRect();
  const std::vector<pp::Rect>& rects =
      find_results_[current_find_index_.GetIndex()].GetScreenRects(
          pp::Point(), 1.0, current_rotation_);
  for (const auto& rect : rects)
    bounding_rect = bounding_rect.Union(rect);
  if (!visible_rect.Contains(bounding_rect)) {
    pp::Point center = bounding_rect.CenterPoint();
    int new_y = CalculateCenterForZoom(center.y(), visible_rect.height(),
                                       current_zoom_);
    client_->ScrollToY(new_y,  false);

    if (center.x() < visible_rect.x() || center.x() > visible_rect.right()) {
      int new_x = CalculateCenterForZoom(center.x(), visible_rect.width(),
                                         current_zoom_);
      client_->ScrollToX(new_x);
    }
  }

  client_->NotifySelectedFindResultChanged(current_find_index_.GetIndex());
  client_->NotifyNumberOfFindResultsChanged(find_results_.size(), true);
  return true;
}
