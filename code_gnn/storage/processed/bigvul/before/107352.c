void Browser::ShowOptionsTab(const std::string& sub_page) {
  GURL url(chrome::kChromeUISettingsURL + sub_page);
  ShowSingletonTab(url, true);
}
