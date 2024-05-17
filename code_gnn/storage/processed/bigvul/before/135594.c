VisibleSelection Editor::SelectionForCommand(Event* event) {
  VisibleSelection selection =
      GetFrame().Selection().ComputeVisibleSelectionInDOMTree();
  if (!event)
    return selection;
  TextControlElement* text_control_of_selection_start =
      EnclosingTextControl(selection.Start());
  TextControlElement* text_control_of_target =
      IsTextControlElement(*event->target()->ToNode())
          ? ToTextControlElement(event->target()->ToNode())
          : nullptr;
  if (text_control_of_target &&
      (selection.Start().IsNull() ||
       text_control_of_target != text_control_of_selection_start)) {
    const SelectionInDOMTree& select = text_control_of_target->Selection();
    if (!select.IsNone())
      return CreateVisibleSelection(select);
  }
  return selection;
}
