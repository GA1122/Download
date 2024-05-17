ViewportDescription Document::GetViewportDescription() const {
  ViewportDescription applied_viewport_description = viewport_description_;
  bool viewport_meta_enabled =
      GetSettings() && GetSettings()->GetViewportMetaEnabled();
  if (legacy_viewport_description_.type !=
          ViewportDescription::kUserAgentStyleSheet &&
      viewport_meta_enabled)
    applied_viewport_description = legacy_viewport_description_;
  if (ShouldOverrideLegacyDescription(viewport_description_.type))
    applied_viewport_description = viewport_description_;

  return applied_viewport_description;
}
