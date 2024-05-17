PDFiumEngine::SelectionChangeInvalidator::~SelectionChangeInvalidator() {
  pp::Point offset = previous_origin_ - engine_->GetVisibleRect().point();
  for (auto& old_selection : old_selections_)
    old_selection.Offset(offset);

  std::vector<pp::Rect> new_selections;
  GetVisibleSelectionsScreenRects(&new_selections);
  for (auto& new_selection : new_selections) {
    for (auto& old_selection : old_selections_) {
      if (!old_selection.IsEmpty() && new_selection == old_selection) {
        new_selection = old_selection = pp::Rect();
        break;
      }
    }
  }

  for (const auto& old_selection : old_selections_) {
    if (!old_selection.IsEmpty())
      engine_->client_->Invalidate(old_selection);
  }
  for (const auto& new_selection : new_selections) {
    if (!new_selection.IsEmpty())
      engine_->client_->Invalidate(new_selection);
  }
  engine_->OnSelectionChanged();
}
