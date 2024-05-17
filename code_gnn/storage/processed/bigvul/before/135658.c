void FrameSelection::NotifyTextControlOfSelectionChange(
    SetSelectionBy set_selection_by) {
  TextControlElement* text_control =
      EnclosingTextControl(GetSelectionInDOMTree().Base());
  if (!text_control)
    return;
  text_control->SelectionChanged(set_selection_by == SetSelectionBy::kUser);
}
