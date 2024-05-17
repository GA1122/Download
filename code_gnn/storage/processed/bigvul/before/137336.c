  int GetCursorPositionX(int cursor_pos) {
    return test_api_->GetRenderText()->GetCursorBounds(
        gfx::SelectionModel(cursor_pos, gfx::CURSOR_FORWARD), false).x();
  }
