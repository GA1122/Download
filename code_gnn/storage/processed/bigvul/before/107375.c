  string16 GetFindBarMatchCountTextForBrowser(Browser* browser) {
    FindBarTesting* find_bar =
      browser->GetFindBarController()->find_bar()->GetFindBarTesting();
    return find_bar->GetMatchCountText();
  }
