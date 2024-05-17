spdy::SettingsMap GetHttp2Settings(
    const VariationParameters& http2_trial_params) {
  spdy::SettingsMap http2_settings;

  const std::string settings_string =
      GetVariationParam(http2_trial_params, "http2_settings");

  base::StringPairs key_value_pairs;
  if (!base::SplitStringIntoKeyValuePairs(settings_string, ':', ',',
                                          &key_value_pairs)) {
    return http2_settings;
  }

  for (auto key_value : key_value_pairs) {
    uint32_t key;
    if (!base::StringToUint(key_value.first, &key))
      continue;
    uint32_t value;
    if (!base::StringToUint(key_value.second, &value))
      continue;
    http2_settings[static_cast<spdy::SpdyKnownSettingsId>(key)] = value;
  }

  return http2_settings;
}
