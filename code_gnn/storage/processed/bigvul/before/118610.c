void NTPResourceCache::CreateNewTabCSS() {
  ui::ThemeProvider* tp = ThemeServiceFactory::GetForProfile(profile_);
  DCHECK(tp);

  SkColor color_background =
      GetThemeColor(tp, ThemeProperties::COLOR_NTP_BACKGROUND);
  SkColor color_text = GetThemeColor(tp, ThemeProperties::COLOR_NTP_TEXT);
  SkColor color_link = GetThemeColor(tp, ThemeProperties::COLOR_NTP_LINK);
  SkColor color_link_underline =
      GetThemeColor(tp, ThemeProperties::COLOR_NTP_LINK_UNDERLINE);

  SkColor color_section =
      GetThemeColor(tp, ThemeProperties::COLOR_NTP_SECTION);
  SkColor color_section_text =
      GetThemeColor(tp, ThemeProperties::COLOR_NTP_SECTION_TEXT);
  SkColor color_section_link =
      GetThemeColor(tp, ThemeProperties::COLOR_NTP_SECTION_LINK);
  SkColor color_section_link_underline =
      GetThemeColor(tp, ThemeProperties::COLOR_NTP_SECTION_LINK_UNDERLINE);
  SkColor color_section_header_text =
      GetThemeColor(tp, ThemeProperties::COLOR_NTP_SECTION_HEADER_TEXT);
  SkColor color_section_header_text_hover =
      GetThemeColor(tp, ThemeProperties::COLOR_NTP_SECTION_HEADER_TEXT_HOVER);
  SkColor color_section_header_rule =
      GetThemeColor(tp, ThemeProperties::COLOR_NTP_SECTION_HEADER_RULE);
  SkColor color_section_header_rule_light =
      GetThemeColor(tp, ThemeProperties::COLOR_NTP_SECTION_HEADER_RULE_LIGHT);
  SkColor color_text_light =
      GetThemeColor(tp, ThemeProperties::COLOR_NTP_TEXT_LIGHT);

  SkColor color_header =
      GetThemeColor(tp, ThemeProperties::COLOR_NTP_HEADER);
  color_utils::HSL header_lighter;
  color_utils::SkColorToHSL(color_header, &header_lighter);
  header_lighter.l += (1 - header_lighter.l) * 0.33;
  SkColor color_header_gradient_light =
      color_utils::HSLToSkColor(header_lighter, SkColorGetA(color_header));

  SkColor color_section_border =
      SkColorSetARGB(80,
                     SkColorGetR(color_header),
                     SkColorGetG(color_header),
                     SkColorGetB(color_header));

  std::vector<std::string> subst;

  subst.push_back(
      profile_->GetPrefs()->GetString(prefs::kCurrentThemeID));   

  subst.push_back(SkColorToRGBAString(color_background));   
  subst.push_back(GetNewTabBackgroundCSS(tp, false));   
  subst.push_back(GetNewTabBackgroundCSS(tp, true));   
  subst.push_back(GetNewTabBackgroundTilingCSS(tp));   
  subst.push_back(SkColorToRGBAString(color_header));   
  subst.push_back(SkColorToRGBAString(color_header_gradient_light));   
  subst.push_back(SkColorToRGBAString(color_text));   
  subst.push_back(SkColorToRGBAString(color_link));   
  subst.push_back(SkColorToRGBAString(color_section));   
  subst.push_back(SkColorToRGBAString(color_section_border));   
  subst.push_back(SkColorToRGBAString(color_section_text));   
  subst.push_back(SkColorToRGBAString(color_section_link));   
  subst.push_back(SkColorToRGBAString(color_link_underline));   
  subst.push_back(SkColorToRGBAString(color_section_link_underline));   
  subst.push_back(SkColorToRGBAString(color_section_header_text));   
  subst.push_back(SkColorToRGBAString(
      color_section_header_text_hover));   
  subst.push_back(SkColorToRGBAString(color_section_header_rule));   
  subst.push_back(SkColorToRGBAString(
      color_section_header_rule_light));   
  subst.push_back(SkColorToRGBAString(
      SkColorSetA(color_section_header_rule, 0)));   
  subst.push_back(SkColorToRGBAString(color_text_light));   
  subst.push_back(SkColorToRGBComponents(color_section_border));   
  subst.push_back(SkColorToRGBComponents(color_text));   

  static const base::StringPiece new_tab_theme_css(
      ResourceBundle::GetSharedInstance().GetRawDataResource(
          IDR_NEW_TAB_4_THEME_CSS));

  std::string css_string;
  css_string = ReplaceStringPlaceholders(new_tab_theme_css, subst, NULL);
  new_tab_css_ = base::RefCountedString::TakeString(&css_string);
}
