void ManifestUmaUtil::ParseSucceeded(const Manifest& manifest) {
  UMA_HISTOGRAM_BOOLEAN(kUMANameParseSuccess, true);
  UMA_HISTOGRAM_BOOLEAN("Manifest.IsEmpty", manifest.IsEmpty());
  if (manifest.IsEmpty())
    return;

  UMA_HISTOGRAM_BOOLEAN("Manifest.HasProperty.name", !manifest.name.is_null());
  UMA_HISTOGRAM_BOOLEAN("Manifest.HasProperty.short_name",
      !manifest.short_name.is_null());
  UMA_HISTOGRAM_BOOLEAN("Manifest.HasProperty.start_url",
      !manifest.start_url.is_empty());
  UMA_HISTOGRAM_BOOLEAN("Manifest.HasProperty.display",
                        manifest.display != blink::kWebDisplayModeUndefined);
  UMA_HISTOGRAM_BOOLEAN(
      "Manifest.HasProperty.orientation",
      manifest.orientation != blink::kWebScreenOrientationLockDefault);
  UMA_HISTOGRAM_BOOLEAN("Manifest.HasProperty.icons", !manifest.icons.empty());
  UMA_HISTOGRAM_BOOLEAN("Manifest.HasProperty.share_target",
                        manifest.share_target.has_value());
  UMA_HISTOGRAM_BOOLEAN("Manifest.HasProperty.gcm_sender_id",
      !manifest.gcm_sender_id.is_null());
}
