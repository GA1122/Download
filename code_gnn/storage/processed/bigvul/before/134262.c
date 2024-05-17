bool OmniboxViewViews::HandleEarlyTabActions(const ui::KeyEvent& event) {
  if (views::FocusManager::IsTabTraversalKeyEvent(event)) {
    if (model()->is_keyword_hint() && !event.IsShiftDown()) {
      model()->AcceptKeyword(ENTERED_KEYWORD_MODE_VIA_TAB);
      return true;
    }
    if (model()->popup_model()->IsOpen()) {
      if (event.IsShiftDown() &&
          model()->popup_model()->selected_line_state() ==
              OmniboxPopupModel::KEYWORD) {
        model()->ClearKeyword(text());
      } else {
        model()->OnUpOrDownKeyPressed(event.IsShiftDown() ? -1 : 1);
      }
      return true;
    }
  }

  return false;
}
