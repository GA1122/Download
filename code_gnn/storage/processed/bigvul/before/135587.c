void Editor::ReplaceSelectionWithFragment(DocumentFragment* fragment,
                                          bool select_replacement,
                                          bool smart_replace,
                                          bool match_style,
                                          InputEvent::InputType input_type) {
  DCHECK(!GetFrame().GetDocument()->NeedsLayoutTreeUpdate());
  const VisibleSelection& selection =
      GetFrame().Selection().ComputeVisibleSelectionInDOMTree();
  if (selection.IsNone() || !selection.IsContentEditable() || !fragment)
    return;

  ReplaceSelectionCommand::CommandOptions options =
      ReplaceSelectionCommand::kPreventNesting |
      ReplaceSelectionCommand::kSanitizeFragment;
  if (select_replacement)
    options |= ReplaceSelectionCommand::kSelectReplacement;
  if (smart_replace)
    options |= ReplaceSelectionCommand::kSmartReplace;
  if (match_style)
    options |= ReplaceSelectionCommand::kMatchStyle;
  DCHECK(GetFrame().GetDocument());
  ReplaceSelectionCommand::Create(*GetFrame().GetDocument(), fragment, options,
                                  input_type)
      ->Apply();
  RevealSelectionAfterEditingOperation();
}
