void AutocompleteEditModel::OnUpOrDownKeyPressed(int count) {

  if (!popup_->IsOpen()) {
    if (!query_in_progress()) {
      if (!user_input_in_progress_)
        InternalSetUserText(permanent_text_);
      view_->UpdatePopup();
    } else {
    }
  } else {
    popup_->Move(count);
  }
}
