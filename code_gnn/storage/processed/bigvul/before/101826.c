void Browser::ShowSingletonTabRespectRef(const GURL& url) {
  browser::NavigateParams params(GetSingletonTabNavigateParams(url));
  params.ref_behavior = browser::NavigateParams::RESPECT_REF;
  browser::Navigate(&params);
}
