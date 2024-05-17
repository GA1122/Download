  gfx::Rect GetCursorBounds(const gfx::SelectionModel& sel) {
    return test_api_->GetRenderText()->GetCursorBounds(sel, true);
  }
