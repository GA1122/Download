std::string GetNewTabBackgroundTilingCSS(
    const ui::ThemeProvider* theme_provider) {
  int repeat_mode;
  theme_provider->GetDisplayProperty(
      ThemeService::NTP_BACKGROUND_TILING, &repeat_mode);
  return ThemeService::TilingToString(repeat_mode);
}
