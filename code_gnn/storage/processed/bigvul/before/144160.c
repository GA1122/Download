bool IsTouchableAppContextMenuEnabled() {
  return base::FeatureList::IsEnabled(kTouchableAppContextMenu) ||
         switches::IsTouchableAppContextMenuEnabled();
}
