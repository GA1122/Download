bool Editor::InsertTextWithoutSendingTextEvent(
    const String& text,
    bool select_inserted_text,
    TextEvent* triggering_event,
    InputEvent::InputType input_type) {
  const VisibleSelection& selection = SelectionForCommand(triggering_event);
  if (!selection.IsContentEditable())
    return false;

  GetSpellChecker().UpdateMarkersForWordsAffectedByEditing(
      !text.IsEmpty() && IsSpaceOrNewline(text[0]));

  TypingCommand::InsertText(
      *selection.Start().GetDocument(), text, selection.AsSelection(),
      select_inserted_text ? TypingCommand::kSelectInsertedText : 0,
      triggering_event && triggering_event->IsComposition()
          ? TypingCommand::kTextCompositionConfirm
          : TypingCommand::kTextCompositionNone,
      false, input_type);

  if (LocalFrame* edited_frame = selection.Start().GetDocument()->GetFrame()) {
    if (Page* page = edited_frame->GetPage()) {
      LocalFrame* focused_or_main_frame =
          ToLocalFrame(page->GetFocusController().FocusedOrMainFrame());
      focused_or_main_frame->Selection().RevealSelection(
          ScrollAlignment::kAlignCenterIfNeeded);
    }
  }

  return true;
}
