std::string GetNewTabBackgroundCSS(const ui::ThemeProvider* theme_provider,
                                   bool bar_attached) {
  if (!theme_provider->HasCustomImage(IDR_THEME_NTP_BACKGROUND)) {
    return "-64px";
  }

  int alignment = theme_provider->GetDisplayProperty(
      ThemeProperties::NTP_BACKGROUND_ALIGNMENT);

  if (bar_attached)
    return ThemeProperties::AlignmentToString(alignment);

  if (alignment & ThemeProperties::ALIGN_TOP) {
    int offset = chrome::kNTPBookmarkBarHeight;

    if (alignment & ThemeProperties::ALIGN_LEFT)
      return "left " + base::IntToString(-offset) + "px";
    else if (alignment & ThemeProperties::ALIGN_RIGHT)
      return "right " + base::IntToString(-offset) + "px";
    return "center " + base::IntToString(-offset) + "px";
  }

  return ThemeProperties::AlignmentToString(alignment);
}
