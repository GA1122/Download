void Editor::PasteAsPlainText(const String& pasting_text, bool smart_replace) {
  Element* target = FindEventTargetFromSelection();
  if (!target)
    return;
  target->DispatchEvent(TextEvent::CreateForPlainTextPaste(
      GetFrame().DomWindow(), pasting_text, smart_replace));
}
