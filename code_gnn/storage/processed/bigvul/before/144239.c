void AddPublicSessionDetailsToUserDictionaryEntry(
    base::DictionaryValue* user_dict,
    const std::vector<std::string>* public_session_recommended_locales) {
  std::string domain;
  if (GetEnterpriseDomain(&domain))
    user_dict->SetString(kKeyEnterpriseDisplayDomain, domain);

  std::string selected_locale;
  bool has_multiple_locales;
  std::unique_ptr<base::ListValue> available_locales =
      GetPublicSessionLocales(public_session_recommended_locales,
                              &selected_locale, &has_multiple_locales);

  user_dict->Set(kKeyInitialLocales, std::move(available_locales));

  user_dict->SetString(kKeyInitialLocale, selected_locale);

  user_dict->SetBoolean(kKeyInitialMultipleRecommendedLocales,
                        has_multiple_locales);
}
