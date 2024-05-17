  string16 GetFindBarTextForBrowser(Browser* browser) {
    FindBarTesting* find_bar =
        browser->GetFindBarController()->find_bar()->GetFindBarTesting();
    return find_bar->GetFindText();
  }
