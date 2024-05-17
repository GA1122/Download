void EnsureTabSelected(Browser* browser, WebContents* tab) {
  TabContentsWrapper* active =
      browser->tabstrip_model()->GetActiveTabContents();
  if (!active || active->web_contents() != tab ||
      browser != BrowserList::GetLastActive()) {
    browser->ActivateTabAt(
        browser->tabstrip_model()->GetIndexOfController(&tab->GetController()),
        true  );
  }
}
