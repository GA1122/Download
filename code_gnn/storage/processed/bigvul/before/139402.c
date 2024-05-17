bool Editor::ExecuteCommand(const String& command_name) {
  DCHECK(GetFrame().GetDocument()->IsActive());
  if (command_name == "DeleteToEndOfParagraph") {
    if (!DeleteWithDirection(DeleteDirection::kForward,
                             TextGranularity::kParagraphBoundary, true,
                             false)) {
      DeleteWithDirection(DeleteDirection::kForward,
                          TextGranularity::kCharacter, true, false);
    }
    return true;
  }
  if (command_name == "DeleteBackward")
    return CreateCommand(AtomicString("BackwardDelete")).Execute();
  if (command_name == "DeleteForward")
    return CreateCommand(AtomicString("ForwardDelete")).Execute();
  if (command_name == "AdvanceToNextMisspelling") {
    GetFrame().GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

    GetSpellChecker().AdvanceToNextMisspelling(false);
    return true;
  }
  if (command_name == "ToggleSpellPanel") {
    GetFrame().GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

    GetSpellChecker().ShowSpellingGuessPanel();
    return true;
  }
  return CreateCommand(command_name).Execute();
}
