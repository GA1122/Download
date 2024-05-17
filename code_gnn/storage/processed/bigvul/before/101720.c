TabContents* Browser::OpenApplicationTab(Profile* profile,
                                         const Extension* extension,
                                         WindowOpenDisposition disposition) {
  Browser* browser = BrowserList::FindTabbedBrowser(profile, false);
  TabContents* contents = NULL;
  if (!browser)
    return contents;

  ExtensionService* extension_service = profile->GetExtensionService();
  DCHECK(extension_service);

  ExtensionPrefs::LaunchType launch_type =
      extension_service->extension_prefs()->GetLaunchType(
          extension->id(), ExtensionPrefs::LAUNCH_DEFAULT);
  UMA_HISTOGRAM_ENUMERATION("Extensions.AppTabLaunchType", launch_type, 100);

  if (extension->id() == extension_misc::kWebStoreAppId)
    UserMetrics::RecordAction(UserMetricsAction("Extensions.WebStoreLaunch"));

  int add_type = TabStripModel::ADD_ACTIVE;
  if (launch_type == ExtensionPrefs::LAUNCH_PINNED)
    add_type |= TabStripModel::ADD_PINNED;

  GURL extension_url = extension->GetFullLaunchURL();
  if (!extension_url.is_valid()) {
    extension_url = extension->options_url();
    if (!extension_url.is_valid())
      extension_url = GURL(std::string(chrome::kChromeUISettingsURL) +
                           chrome::kExtensionsSubPage);
  }

  browser::NavigateParams params(browser, extension_url,
                                 PageTransition::START_PAGE);
  params.tabstrip_add_types = add_type;
  params.disposition = disposition;

  if (disposition == CURRENT_TAB) {
    TabContents* existing_tab = browser->GetSelectedTabContents();
    TabStripModel* model = browser->tabstrip_model();
    int tab_index = model->GetWrapperIndex(existing_tab);

    existing_tab->OpenURL(extension->GetFullLaunchURL(), existing_tab->GetURL(),
                          disposition, PageTransition::LINK);
    if (params.tabstrip_add_types & TabStripModel::ADD_PINNED) {
      model->SetTabPinned(tab_index, true);
      tab_index = model->GetWrapperIndex(existing_tab);
    }
    if (params.tabstrip_add_types & TabStripModel::ADD_ACTIVE)
      model->ActivateTabAt(tab_index, true);

    contents = existing_tab;
  } else {
    browser::Navigate(&params);
    contents = params.target_contents->tab_contents();
  }

  if (launch_type == ExtensionPrefs::LAUNCH_FULLSCREEN &&
      !browser->window()->IsFullscreen())
    browser->ToggleFullscreenMode();

  return contents;
}
