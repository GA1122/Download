std::string ChromeContentBrowserClient::GetAcceptLangs(const TabContents* tab) {
  return tab->profile()->GetPrefs()->GetString(prefs::kAcceptLanguages);
}
