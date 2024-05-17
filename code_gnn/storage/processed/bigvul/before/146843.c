bool Document::ShouldMergeWithLegacyDescription(
    ViewportDescription::Type origin) const {
  return GetSettings() && GetSettings()->GetViewportMetaMergeContentQuirk() &&
         legacy_viewport_description_.IsMetaViewportType() &&
         legacy_viewport_description_.type == origin;
}
