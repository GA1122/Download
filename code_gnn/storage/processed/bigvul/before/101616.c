void Browser::CreateHistoricalTab(TabContentsWrapper* contents) {
  if (!profile() || profile()->IsOffTheRecord())
    return;

  if (contents->tab_contents()->GetURL() == GURL(chrome::kChromeUIPrintURL))
    return;

  TabRestoreService* service =
      TabRestoreServiceFactory::GetForProfile(profile());

  if (service && CanSupportWindowFeature(FEATURE_TABSTRIP)) {
    service->CreateHistoricalTab(&contents->controller(),
        tab_handler_->GetTabStripModel()->GetIndexOfTabContents(contents));
  }
}
