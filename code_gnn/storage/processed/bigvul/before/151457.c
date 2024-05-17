bool FrameFetchContext::ShouldSendClientHint(
    mojom::WebClientHintsType type,
    const ClientHintsPreferences& hints_preferences,
    const WebEnabledClientHints& enabled_hints) const {
  return GetClientHintsPreferences().ShouldSend(type) ||
         hints_preferences.ShouldSend(type) || enabled_hints.IsEnabled(type);
}
