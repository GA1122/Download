void BrowserInit::LaunchWithProfile::AddInfoBarsIfNecessary(Browser* browser) {
  if (!browser || !profile_ || browser->tab_count() == 0)
    return;

  TabContentsWrapper* tab_contents = browser->GetSelectedTabContentsWrapper();
  AddCrashedInfoBarIfNecessary(browser, tab_contents);
  AddBadFlagsInfoBarIfNecessary(tab_contents);
  AddDNSCertProvenanceCheckingWarningInfoBarIfNecessary(tab_contents);
  AddObsoleteSystemInfoBarIfNecessary(tab_contents);
}
