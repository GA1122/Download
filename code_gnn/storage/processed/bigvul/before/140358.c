void Editor::replaceSelectionWithFragment(DocumentFragment* fragment,
                                          bool selectReplacement,
                                          bool smartReplace,
                                          bool matchStyle,
                                          InputEvent::InputType inputType) {
  DCHECK(!frame().document()->needsLayoutTreeUpdate());
  if (frame()
          .selection()
          .computeVisibleSelectionInDOMTreeDeprecated()
          .isNone() ||
      !frame()
           .selection()
           .computeVisibleSelectionInDOMTreeDeprecated()
           .isContentEditable() ||
      !fragment)
    return;

  ReplaceSelectionCommand::CommandOptions options =
      ReplaceSelectionCommand::PreventNesting |
      ReplaceSelectionCommand::SanitizeFragment;
  if (selectReplacement)
    options |= ReplaceSelectionCommand::SelectReplacement;
  if (smartReplace)
    options |= ReplaceSelectionCommand::SmartReplace;
  if (matchStyle)
    options |= ReplaceSelectionCommand::MatchStyle;
  DCHECK(frame().document());
  ReplaceSelectionCommand::create(*frame().document(), fragment, options,
                                  inputType)
      ->apply();
  revealSelectionAfterEditingOperation();
}
