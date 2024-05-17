void Browser::ShowSingletonTabOverwritingNTP(
    const browser::NavigateParams& params) {
  browser::NavigateParams local_params(params);
  TabContents* contents = GetSelectedTabContents();
  if (contents) {
    const GURL& contents_url = contents->GetURL();
    if ((contents_url == GURL(chrome::kChromeUINewTabURL) ||
         contents_url == GURL(chrome::kAboutBlankURL)) &&
        browser::GetIndexOfSingletonTab(&local_params) < 0) {
      local_params.disposition = CURRENT_TAB;
    }
  }

  browser::Navigate(&local_params);
}
