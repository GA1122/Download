bool IsVoiceInteractionLocalesSupported() {
  if (base::FeatureList::IsEnabled(kAssistantFeatureForLocale))
    return true;


  std::string kLocale = icu::Locale::getDefault().getName();
  if (kLocale != ULOC_US && kLocale != ULOC_UK && kLocale != ULOC_CANADA &&
      base::CommandLine::ForCurrentProcess()
              ->GetSwitchValueASCII(
                  chromeos::switches::kVoiceInteractionLocales)
              .find(kLocale) == std::string::npos) {
    return false;
  }
  return true;
}
