void Editor::ApplyStyle(StylePropertySet* style,
                        InputEvent::InputType input_type) {
  const VisibleSelection& selection =
      GetFrame().Selection().ComputeVisibleSelectionInDOMTreeDeprecated();
  if (selection.IsNone())
    return;
  if (selection.IsCaret()) {
    ComputeAndSetTypingStyle(style, input_type);
    return;
  }
  DCHECK(selection.IsRange()) << selection;
  if (!style)
    return;
  DCHECK(GetFrame().GetDocument());
  ApplyStyleCommand::Create(*GetFrame().GetDocument(),
                            EditingStyle::Create(style), input_type)
      ->Apply();
}
