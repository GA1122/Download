std::unique_ptr<base::ListValue> GetPublicSessionLocales(
    const std::vector<std::string>* public_session_recommended_locales,
    std::string* out_selected_locale,
    bool* out_multiple_locales) {
  std::vector<std::string> kEmptyRecommendedLocales;
  const std::vector<std::string>& recommended_locales =
      public_session_recommended_locales ? *public_session_recommended_locales
                                         : kEmptyRecommendedLocales;

  std::unique_ptr<base::ListValue> available_locales =
      GetUILanguageList(&recommended_locales, std::string());

  *out_selected_locale =
      FindMostRelevantLocale(recommended_locales, *available_locales.get(),
                             g_browser_process->GetApplicationLocale());

  *out_multiple_locales = recommended_locales.size() >= 2;
  return available_locales;
}
