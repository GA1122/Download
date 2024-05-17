  virtual void Run() {
    Browser* browser = BrowserList::GetLastActive();
    if (!browser) {
      return;
    }
    TabContents* tab = browser->GetSelectedTabContents();
    if (!tab || tab->infobar_delegate_count() > 0)
      return;
    tab->AddInfoBar(new DefaultBrowserInfoBarDelegate(tab));
  }
