void Tab::ActiveStateChanged() {
  UpdateTabIconNeedsAttentionBlocked();
  UpdateForegroundColors();
  Layout();
}
