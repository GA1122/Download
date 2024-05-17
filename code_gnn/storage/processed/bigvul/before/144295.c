std::string GetManagedLoginScreenLocale() {
  chromeos::CrosSettings* cros_settings = chromeos::CrosSettings::Get();
  const base::ListValue* login_screen_locales = nullptr;
  if (!cros_settings->GetList(chromeos::kDeviceLoginScreenLocales,
                              &login_screen_locales))
    return std::string();

  std::string login_screen_locale;
  if (login_screen_locales->empty() ||
      !login_screen_locales->GetString(0, &login_screen_locale))
    return std::string();

  return login_screen_locale;
}
