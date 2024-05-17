void Browser::ShowSingletonTab(const GURL& url) {
  browser::NavigateParams params(GetSingletonTabNavigateParams(url));
  browser::Navigate(&params);
}
