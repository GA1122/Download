TabContents* Browser::OpenApplicationTab(Profile* profile,
                                         const Extension* extension,
                                         TabContents* existing_tab) {
  Browser* browser = BrowserList::GetLastActiveWithProfile(profile);
  TabContents* contents = NULL;
  if (!browser || browser->type() != Browser::TYPE_NORMAL)
    return contents;

  ExtensionService* extensions_service = profile->GetExtensionService();
  DCHECK(extensions_service);

  ExtensionPrefs::LaunchType launch_type =
      extensions_service->extension_prefs()->GetLaunchType(
          extension->id(), ExtensionPrefs::LAUNCH_REGULAR);
  UMA_HISTOGRAM_ENUMERATION("Extensions.AppTabLaunchType", launch_type, 100);
  int add_type = TabStripModel::ADD_SELECTED;
  if (launch_type == ExtensionPrefs::LAUNCH_PINNED)
    add_type |= TabStripModel::ADD_PINNED;

  browser::NavigateParams params(browser, extension->GetFullLaunchURL(),
                                 PageTransition::START_PAGE);
  params.tabstrip_add_types = add_type;

  if (existing_tab) {
    TabStripModel* model = browser->tabstrip_model();
    int tab_index = model->GetWrapperIndex(existing_tab);

    existing_tab->OpenURL(extension->GetFullLaunchURL(), existing_tab->GetURL(),
                          CURRENT_TAB, PageTransition::LINK);
    if (params.tabstrip_add_types & TabStripModel::ADD_PINNED) {
      model->SetTabPinned(tab_index, true);
      tab_index = model->GetWrapperIndex(existing_tab);
    }
    if (params.tabstrip_add_types & TabStripModel::ADD_SELECTED)
      model->SelectTabContentsAt(tab_index, true);

    contents = existing_tab;
  } else {
    params.disposition = NEW_FOREGROUND_TAB;
    browser::Navigate(&params);
    contents = params.target_contents->tab_contents();
  }

  if (launch_type == ExtensionPrefs::LAUNCH_FULLSCREEN)
    browser->window()->SetFullscreen(true);

  return contents;
}