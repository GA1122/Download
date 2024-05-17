bool ShouldRemoveBookmarkThisPageUI(Profile* profile) {
  return GetBookmarkShortcutDisposition(profile) ==
         BOOKMARK_SHORTCUT_DISPOSITION_REMOVED;
}
