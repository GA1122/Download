void Browser::ShowOptionsTab(const std::string& sub_page) {
  browser::NavigateParams params(GetSingletonTabNavigateParams(
      GURL(chrome::kChromeUISettingsURL + sub_page)));
  params.path_behavior = browser::NavigateParams::IGNORE_AND_NAVIGATE;

  ShowSingletonTabOverwritingNTP(params);
}
