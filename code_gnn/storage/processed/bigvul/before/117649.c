void NotifyNotDefaultBrowserCallback() {
  Browser* browser = BrowserList::GetLastActive();
  if (!browser)
    return;   

  TabContentsWrapper* tab = browser->GetSelectedTabContentsWrapper();
  if (!tab)
    return;

  InfoBarTabHelper* infobar_helper = tab->infobar_tab_helper();
  if (infobar_helper->infobar_count() > 0)
    return;

  infobar_helper->AddInfoBar(
      new DefaultBrowserInfoBarDelegate(infobar_helper,
                                        tab->profile()->GetPrefs()));
}
