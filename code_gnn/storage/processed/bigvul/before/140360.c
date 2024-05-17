void Editor::respondToChangedContents(const Position& position) {
  if (frame().settings() && frame().settings()->getAccessibilityEnabled()) {
    Node* node = position.anchorNode();
    if (AXObjectCache* cache = frame().document()->existingAXObjectCache())
      cache->handleEditableTextContentChanged(node);
  }

  spellChecker().respondToChangedContents();
  client().respondToChangedContents();
}
