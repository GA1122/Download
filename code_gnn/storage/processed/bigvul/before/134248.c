ui::TextInputType DetermineTextInputType() {
#if defined(OS_WIN)
  DCHECK(g_browser_process);
  const std::string& locale = g_browser_process->GetApplicationLocale();
  const std::string& language = locale.substr(0, 2);
  if (language == "ja" ||
      language == "ko" ||
      language == "th" ||
      language == "zh")
    return ui::TEXT_INPUT_TYPE_SEARCH;
#endif
  return ui::TEXT_INPUT_TYPE_URL;
}
