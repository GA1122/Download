bool Editor::handleTextEvent(TextEvent* event) {
  if (event->isDrop())
    return false;

  if (event->isIncrementalInsertion())
    return false;

  m_frame->document()->updateStyleAndLayoutIgnorePendingStylesheets();

  if (event->isPaste()) {
    if (event->pastingFragment()) {
      replaceSelectionWithFragment(
          event->pastingFragment(), false, event->shouldSmartReplace(),
          event->shouldMatchStyle(), InputEvent::InputType::InsertFromPaste);
    } else {
      replaceSelectionWithText(event->data(), false,
                               event->shouldSmartReplace(),
                               InputEvent::InputType::InsertFromPaste);
    }
    return true;
  }

  String data = event->data();
  if (data == "\n") {
    if (event->isLineBreak())
      return insertLineBreak();
    return insertParagraphSeparator();
  }

  if (data == " " && !canEditRichly() &&
      isCaretAtStartOfWrappedLine(frame().selection())) {
    insertLineBreak();
  }

  return insertTextWithoutSendingTextEvent(data, false, event);
}
