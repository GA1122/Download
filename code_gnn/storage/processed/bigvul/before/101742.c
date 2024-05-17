bool Browser::OpenInstant(WindowOpenDisposition disposition) {
  if (!instant() || !instant()->is_active() || !instant()->IsCurrent() ||
      disposition == NEW_BACKGROUND_TAB) {
    return false;
  }

  if (disposition == CURRENT_TAB) {
    instant()->CommitCurrentPreview(INSTANT_COMMIT_PRESSED_ENTER);
    return true;
  }
  if (disposition == NEW_FOREGROUND_TAB) {
    TabContentsWrapper* preview_contents = instant()->ReleasePreviewContents(
        INSTANT_COMMIT_PRESSED_ENTER);
    HideInstant();
    preview_contents->controller().PruneAllButActive();
    tab_handler_->GetTabStripModel()->AddTabContents(
        preview_contents,
        -1,
        instant()->last_transition_type(),
        TabStripModel::ADD_ACTIVE);
    instant()->CompleteRelease(preview_contents);
    return true;
  }
  NOTREACHED();
  return false;
}
