void FrameFetchContext::AddClientHintsIfNecessary(
    const ClientHintsPreferences& hints_preferences,
    const FetchParameters::ResourceWidth& resource_width,
    ResourceRequest& request) {
  WebEnabledClientHints enabled_hints;

  if (!ClientHintsPreferences::IsClientHintsAllowed(request.Url()))
    return;

  if (!AllowScriptFromSourceWithoutNotifying(request.Url()))
    return;

  bool is_1p_origin = IsFirstPartyOrigin(request.Url());

  if (!base::FeatureList::IsEnabled(kAllowClientHintsToThirdParty) &&
      !is_1p_origin) {
    return;
  }
  if (is_1p_origin && GetContentSettingsClient()) {
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

  if (!is_1p_origin) {
    return;
  }

  if (ShouldSendClientHint(mojom::WebClientHintsType::kRtt, hints_preferences,
                           enabled_hints)) {
    unsigned long rtt = GetNetworkStateNotifier().RoundRtt(
        request.Url().Host(), GetNetworkStateNotifier().HttpRtt());
    request.AddHTTPHeaderField(
        blink::kClientHintsHeaderMapping[static_cast<size_t>(
            mojom::WebClientHintsType::kRtt)],
        AtomicString(String::Number(rtt)));
  }

  if (ShouldSendClientHint(mojom::WebClientHintsType::kDownlink,
                           hints_preferences, enabled_hints)) {
    double mbps = GetNetworkStateNotifier().RoundMbps(
        request.Url().Host(),
        GetNetworkStateNotifier().DownlinkThroughputMbps());
    request.AddHTTPHeaderField(
        blink::kClientHintsHeaderMapping[static_cast<size_t>(
            mojom::WebClientHintsType::kDownlink)],
        AtomicString(String::Number(mbps)));
  }

  if (ShouldSendClientHint(mojom::WebClientHintsType::kEct, hints_preferences,
                           enabled_hints)) {
    request.AddHTTPHeaderField(
        blink::kClientHintsHeaderMapping[static_cast<size_t>(
            mojom::WebClientHintsType::kEct)],
        AtomicString(NetworkStateNotifier::EffectiveConnectionTypeToString(
            GetNetworkStateNotifier().EffectiveType())));
  }
}
