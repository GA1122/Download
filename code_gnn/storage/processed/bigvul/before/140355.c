void Editor::replaceSelection(const String& text) {
  DCHECK(!frame().document()->needsLayoutTreeUpdate());
  bool selectReplacement = behavior().shouldSelectReplacement();
  bool smartReplace = true;
  replaceSelectionWithText(text, selectReplacement, smartReplace,
                           InputEvent::InputType::InsertReplacementText);
}
