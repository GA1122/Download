std::string GetNewTabBackgroundTilingCSS(
    const ui::ThemeProvider* theme_provider) {
  int repeat_mode = theme_provider->GetDisplayProperty(
      ThemeProperties::NTP_BACKGROUND_TILING);
  return ThemeProperties::TilingToString(repeat_mode);
}
