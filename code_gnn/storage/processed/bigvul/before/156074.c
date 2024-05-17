std::string GetGoogleLocale(const std::string& application_locale) {
  return (application_locale == "nb") ? "no" : application_locale;
}
