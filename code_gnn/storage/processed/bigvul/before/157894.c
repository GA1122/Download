void ApplyBlinkSettings(const base::CommandLine& command_line,
                        WebSettings* settings) {
  if (!command_line.HasSwitch(switches::kBlinkSettings))
    return;

  std::vector<std::string> blink_settings = base::SplitString(
      command_line.GetSwitchValueASCII(switches::kBlinkSettings),
      ",", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);
  for (const std::string& setting : blink_settings) {
    size_t pos = setting.find('=');
    settings->SetFromStrings(
        blink::WebString::FromLatin1(setting.substr(0, pos)),
        blink::WebString::FromLatin1(
            pos == std::string::npos ? "" : setting.substr(pos + 1)));
  }
}
