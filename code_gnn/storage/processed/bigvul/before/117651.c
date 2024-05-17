Browser* BrowserInit::LaunchWithProfile::OpenTabsInBrowser(
        Browser* browser,
        bool process_startup,
        const std::vector<Tab>& tabs) {
  DCHECK(!tabs.empty());
  if (!profile_ && browser)
    profile_ = browser->profile();

  if (!browser || !browser->is_type_tabbed()) {
    browser = Browser::Create(profile_);
  } else {
#if defined(TOOLKIT_GTK)
    gtk_util::SetWMLastUserActionTime(browser->window()->GetNativeHandle());
#endif
  }

#if !defined(OS_MACOSX)
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kKioskMode))
    browser->ToggleFullscreenMode(false);
#endif

  bool first_tab = true;
  for (size_t i = 0; i < tabs.size(); ++i) {
    bool handled_by_chrome = ProfileIOData::IsHandledURL(tabs[i].url) ||
        (profile_ && profile_->GetProtocolHandlerRegistry()->IsHandledProtocol(
            tabs[i].url.scheme()));
    if (!process_startup && !handled_by_chrome)
      continue;

    int add_types = first_tab ? TabStripModel::ADD_ACTIVE :
                                TabStripModel::ADD_NONE;
    add_types |= TabStripModel::ADD_FORCE_INDEX;
    if (tabs[i].is_pinned)
      add_types |= TabStripModel::ADD_PINNED;
    int index = browser->GetIndexForInsertionDuringRestore(i);

    browser::NavigateParams params(browser, tabs[i].url,
                                   content::PAGE_TRANSITION_START_PAGE);
    params.disposition = first_tab ? NEW_FOREGROUND_TAB : NEW_BACKGROUND_TAB;
    params.tabstrip_index = index;
    params.tabstrip_add_types = add_types;
    params.extension_app_id = tabs[i].app_id;
    browser::Navigate(&params);

    first_tab = false;
  }
  browser->window()->Show();
  browser->GetSelectedWebContents()->GetView()->SetInitialFocus();

  return browser;
}
