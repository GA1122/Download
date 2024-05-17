BrowserEventRouter::BrowserEventRouter(Profile* profile)
    : profile_(profile) {
  DCHECK(!profile->IsOffTheRecord());

  BrowserList::AddObserver(this);

  for (BrowserList::const_iterator iter = BrowserList::begin();
       iter != BrowserList::end(); ++iter) {
    RegisterForBrowserNotifications(*iter);

    Browser* browser = *iter;
    if (browser->tab_strip_model()) {
      for (int i = 0; i < browser->tab_strip_model()->count(); ++i) {
        WebContents* contents = chrome::GetWebContentsAt(browser, i);
        int tab_id = ExtensionTabUtil::GetTabId(contents);
        tab_entries_[tab_id] = TabEntry();
      }
    }
  }
}
