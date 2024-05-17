void Textfield::OnAfterPointerAction(bool text_changed,
                                     bool selection_changed) {
  OnAfterUserAction();
  UpdateAfterChange(text_changed, selection_changed);
}
