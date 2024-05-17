 void ExtensionTtsPlatformImplChromeOs::AppendSpeakOption(
    std::string key,
    std::string value,
    std::string* options) {
  *options +=
      key +
      chromeos::SpeechSynthesisLibrary::kSpeechPropertyEquals +
      value +
      chromeos::SpeechSynthesisLibrary::kSpeechPropertyDelimiter;
}
