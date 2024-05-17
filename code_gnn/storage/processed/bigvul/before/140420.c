bool PermissionUtil::ShouldShowPersistenceToggle() {
  return base::FeatureList::IsEnabled(
      features::kDisplayPersistenceToggleInPermissionPrompts);
}
