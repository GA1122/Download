  Browser* FinishedTabCreation(bool succeeded, bool created_tabbed_browser) {
    Browser* browser = NULL;
    if (!created_tabbed_browser && always_create_tabbed_browser_) {
      browser = Browser::Create(profile_);
      if (urls_to_open_.empty()) {
        urls_to_open_.push_back(GURL());
      }
      AppendURLsToBrowser(browser, urls_to_open_);
      browser->window()->Show();
    }

    if (succeeded) {
      DCHECK(tab_loader_.get());
      tab_loader_.release()->StartLoading();
    }

    if (!synchronous_) {
      MessageLoop::current()->DeleteSoon(FROM_HERE, this);
    }

#if defined(OS_CHROMEOS)
    chromeos::BootTimesLoader::Get()->AddLoginTimeMarker(
        "SessionRestore-End", false);
#endif
    return browser;
  }
