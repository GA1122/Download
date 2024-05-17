bool Editor::HandleTextEvent(TextEvent* event) {
  if (event->IsDrop())
    return false;

  if (event->IsIncrementalInsertion())
    return false;

  frame_->GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  if (event->IsPaste()) {
    if (event->PastingFragment()) {
      ReplaceSelectionWithFragment(
          event->PastingFragment(), false, event->ShouldSmartReplace(),
          event->ShouldMatchStyle(), InputEvent::InputType::kInsertFromPaste);
    } else {
      ReplaceSelectionWithText(event->data(), false,
                               event->ShouldSmartReplace(),
                               InputEvent::InputType::kInsertFromPaste);
    }
    return true;
  }

  String data = event->data();
  if (data == "\n") {
    if (event->IsLineBreak())
      return InsertLineBreak();
    return InsertParagraphSeparator();
  }

  if (data == " " && !CanEditRichly() &&
      IsCaretAtStartOfWrappedLine(GetFrame().Selection())) {
    InsertLineBreak();
  }

  return InsertTextWithoutSendingTextEvent(data, false, event);
}
