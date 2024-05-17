void Editor::pasteAsFragment(DocumentFragment* pastingFragment,
                             bool smartReplace,
                             bool matchStyle) {
  Element* target = findEventTargetFromSelection();
  if (!target)
    return;
  target->dispatchEvent(TextEvent::createForFragmentPaste(
      frame().domWindow(), pastingFragment, smartReplace, matchStyle));
}
