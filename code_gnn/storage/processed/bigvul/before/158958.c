PDFiumEngine::SelectionChangeInvalidator::GetVisibleSelections() const {
  std::vector<pp::Rect> rects;
  pp::Point visible_point = engine_->GetVisibleRect().point();
  for (auto& range : engine_->selection_) {
    if (!engine_->IsPageVisible(range.page_index()))
      continue;

    const std::vector<pp::Rect>& selection_rects = range.GetScreenRects(
        visible_point, engine_->current_zoom_, engine_->current_rotation_);
    rects.insert(rects.end(), selection_rects.begin(), selection_rects.end());
  }
  return rects;
}
