void Editor::ReplaceSelection(const String& text) {
  DCHECK(!GetFrame().GetDocument()->NeedsLayoutTreeUpdate());
  bool select_replacement = Behavior().ShouldSelectReplacement();
  bool smart_replace = true;
  ReplaceSelectionWithText(text, select_replacement, smart_replace,
                           InputEvent::InputType::kInsertReplacementText);
}
