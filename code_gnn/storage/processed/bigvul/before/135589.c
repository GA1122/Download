void Editor::RespondToChangedContents(const Position& position) {
  if (GetFrame().GetSettings() &&
      GetFrame().GetSettings()->GetAccessibilityEnabled()) {
    Node* node = position.AnchorNode();
    if (AXObjectCache* cache =
            GetFrame().GetDocument()->ExistingAXObjectCache())
      cache->HandleEditableTextContentChanged(node);
  }

  GetSpellChecker().RespondToChangedContents();
  Client().RespondToChangedContents();
}
