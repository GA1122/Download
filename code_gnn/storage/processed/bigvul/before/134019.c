void ExtensionAppItem::OnExtensionPreferenceChanged() {
  if (has_overlay_ != NeedsOverlay())
    UpdateIcon();
}
