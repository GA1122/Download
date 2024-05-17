void Browser::OpenURLFromTab(TabContents* source,
                             const GURL& url,
                             const GURL& referrer,
                             WindowOpenDisposition disposition,
                             PageTransition::Type transition) {
  browser::NavigateParams params(this, url, transition);
  params.source_contents =
    tabstrip_model()->GetTabContentsAt(
      tabstrip_model()->GetWrapperIndex(source));
  params.referrer = referrer;
  params.disposition = disposition;
  params.tabstrip_add_types = TabStripModel::ADD_NONE;
  params.show_window = true;
  browser::Navigate(&params);
}
