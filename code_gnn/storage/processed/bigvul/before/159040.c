PDFiumEngine::SelectionChangeInvalidator::~SelectionChangeInvalidator() {
  pp::Point offset = previous_origin_ - engine_->GetVisibleRect().point();
  for (auto& old_selection : old_selections_)
    old_selection.Offset(offset);

  std::vector<pp::Rect> new_selections = GetVisibleSelections();
  for (auto& new_selection : new_selections) {
    for (auto& old_selection : old_selections_) {
      if (!old_selection.IsEmpty() && new_selection == old_selection) {
        new_selection = old_selection = pp::Rect();
        break;
      }
    }
  }

  bool selection_changed = false;
  for (const auto& old_selection : old_selections_) {
    if (!old_selection.IsEmpty()) {
      Invalidate(old_selection);
      selection_changed = true;
    }
  }
  for (const auto& new_selection : new_selections) {
    if (!new_selection.IsEmpty()) {
      Invalidate(new_selection);
      selection_changed = true;
    }
  }

  if (selection_changed)
    engine_->OnSelectionChanged();
}
