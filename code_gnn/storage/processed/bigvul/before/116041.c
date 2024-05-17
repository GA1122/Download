void NTPResourceCache::CreateNewTabIncognitoCSS() {
  ui::ThemeProvider* tp = ThemeServiceFactory::GetForProfile(profile_);
  DCHECK(tp);

  SkColor color_background =
      tp->GetColor(ThemeService::COLOR_NTP_BACKGROUND);

  std::vector<std::string> subst;

  subst.push_back(
      profile_->GetPrefs()->GetString(prefs::kCurrentThemeID));   

  subst.push_back(SkColorToRGBAString(color_background));   
  subst.push_back(GetNewTabBackgroundCSS(tp, false));   
  subst.push_back(GetNewTabBackgroundCSS(tp, true));   
  subst.push_back(GetNewTabBackgroundTilingCSS(tp));   

  static const base::StringPiece new_tab_theme_css(
      ResourceBundle::GetSharedInstance().GetRawDataResource(
      IDR_NEW_INCOGNITO_TAB_THEME_CSS));

  std::string full_css = ReplaceStringPlaceholders(
      new_tab_theme_css, subst, NULL);

  new_tab_incognito_css_ = base::RefCountedString::TakeString(&full_css);
}
