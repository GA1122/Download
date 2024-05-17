bool Editor::canSmartReplaceWithPasteboard(Pasteboard* pasteboard) {
  return smartInsertDeleteEnabled() && pasteboard->canSmartReplace();
}
