bool OmniboxViewViews::HandleEarlyTabActions(const ui::KeyEvent& event) {
  if (!views::FocusManager::IsTabTraversalKeyEvent(event))
    return false;

  if (model()->is_keyword_hint() && !event.IsShiftDown())
    return model()->AcceptKeyword(KeywordModeEntryMethod::TAB);

  if (!model()->popup_model()->IsOpen())
    return false;

  if (event.IsShiftDown() &&
      (model()->popup_model()->selected_line_state() ==
          OmniboxPopupModel::KEYWORD))
    model()->ClearKeyword();
  else
    model()->OnUpOrDownKeyPressed(event.IsShiftDown() ? -1 : 1);

  return true;
}
