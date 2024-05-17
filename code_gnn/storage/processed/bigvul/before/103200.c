void Browser::ShowOptionsTab(const std::string& sub_page) {
  GURL url(chrome::kChromeUISettingsURL + sub_page);
  browser::NavigateParams params(GetSingletonTabNavigateParams(url));
  params.path_behavior = browser::NavigateParams::IGNORE_AND_NAVIGATE;
  browser::Navigate(&params);
}
