void Editor::PasteAsFragment(DocumentFragment* pasting_fragment,
                             bool smart_replace,
                             bool match_style) {
  Element* target = FindEventTargetFromSelection();
  if (!target)
    return;
  target->DispatchEvent(TextEvent::CreateForFragmentPaste(
      GetFrame().DomWindow(), pasting_fragment, smart_replace, match_style));
}
