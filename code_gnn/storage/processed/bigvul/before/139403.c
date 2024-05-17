bool Editor::ExecuteCommand(const String& command_name, const String& value) {
  DCHECK(GetFrame().GetDocument()->IsActive());
  if (!CanEdit() && command_name == "moveToBeginningOfDocument")
    return GetFrame().GetEventHandler().BubblingScroll(
        kScrollUpIgnoringWritingMode, kScrollByDocument);

  if (!CanEdit() && command_name == "moveToEndOfDocument")
    return GetFrame().GetEventHandler().BubblingScroll(
        kScrollDownIgnoringWritingMode, kScrollByDocument);

  if (command_name == "showGuessPanel") {
    GetFrame().GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

    GetSpellChecker().ShowSpellingGuessPanel();
    return true;
  }

  return CreateCommand(command_name).Execute(value);
}
