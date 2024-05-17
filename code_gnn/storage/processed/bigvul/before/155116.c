bool OmniboxViewViews::HandleEarlyTabActions(const ui::KeyEvent& event) {
  if (!views::FocusManager::IsTabTraversalKeyEvent(event))
    return false;

  if (model()->is_keyword_hint() && !event.IsShiftDown())
    return model()->AcceptKeyword(OmniboxEventProto::TAB);

  if (!model()->popup_model()->IsOpen())
    return false;

  if (event.IsShiftDown() && (model()->popup_model()->selected_line_state() ==
                              OmniboxPopupModel::KEYWORD)) {
    model()->ClearKeyword();
    return true;
  }

  if (!event.IsShiftDown()) {
    if (MaybeFocusTabButton())
      return true;
  }

  if (event.IsShiftDown()) {
    if (MaybeUnfocusTabButton())
      return true;

    if (model()->popup_model()->selected_line() == 0)
      return false;
  }

  model()->OnUpOrDownKeyPressed(event.IsShiftDown() ? -1 : 1);
  if (event.IsShiftDown() &&
      model()->popup_model()->SelectedLineHasButton()) {
    model()->popup_model()->SetSelectedLineState(
        OmniboxPopupModel::BUTTON_FOCUSED);
    popup_view_->ProvideButtonFocusHint(
        model()->popup_model()->selected_line());
  }

  return true;
}
