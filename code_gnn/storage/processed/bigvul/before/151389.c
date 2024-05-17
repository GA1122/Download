void FrameFetchContext::AddClientHintsIfNecessary(
    const ClientHintsPreferences& hints_preferences,
    const FetchParameters::ResourceWidth& resource_width,
    ResourceRequest& request) {
  if (!RuntimeEnabledFeatures::ClientHintsEnabled())
    return;

  WebEnabledClientHints enabled_hints;
  if (blink::RuntimeEnabledFeatures::ClientHintsPersistentEnabled() &&
      GetContentSettingsClient()) {
    GetContentSettingsClient()->GetAllowedClientHintsFromSource(request.Url(),
                                                                &enabled_hints);
  }

  if (ShouldSendClientHint(mojom::WebClientHintsType::kDeviceMemory,
                           hints_preferences, enabled_hints)) {
    request.AddHTTPHeaderField(
        "Device-Memory",
        AtomicString(String::Number(
            ApproximatedDeviceMemory::GetApproximatedDeviceMemory())));
  }

  float dpr = GetDevicePixelRatio();
  if (ShouldSendClientHint(mojom::WebClientHintsType::kDpr, hints_preferences,
                           enabled_hints)) {
    request.AddHTTPHeaderField("DPR", AtomicString(String::Number(dpr)));
  }

  if (ShouldSendClientHint(mojom::WebClientHintsType::kResourceWidth,
                           hints_preferences, enabled_hints)) {
    if (resource_width.is_set) {
      float physical_width = resource_width.width * dpr;
      request.AddHTTPHeaderField(
          "Width", AtomicString(String::Number(ceil(physical_width))));
    }
  }

  if (ShouldSendClientHint(mojom::WebClientHintsType::kViewportWidth,
                           hints_preferences, enabled_hints) &&
      !IsDetached() && GetFrame()->View()) {
    request.AddHTTPHeaderField(
        "Viewport-Width",
        AtomicString(String::Number(GetFrame()->View()->ViewportWidth())));
  }
}
