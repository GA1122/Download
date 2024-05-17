void Editor::pasteAsPlainText(const String& pastingText, bool smartReplace) {
  Element* target = findEventTargetFromSelection();
  if (!target)
    return;
  target->dispatchEvent(TextEvent::createForPlainTextPaste(
      frame().domWindow(), pastingText, smartReplace));
}
