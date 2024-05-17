bool Editor::CanSmartReplaceWithPasteboard(Pasteboard* pasteboard) {
  return SmartInsertDeleteEnabled() && pasteboard->CanSmartReplace();
}
