bool WindowCanOpenTabs(Browser* browser) {
  if (browser->tab_count() >= browser_defaults::kMaxTabCount)
    return false;

  return browser->CanSupportWindowFeature(Browser::FEATURE_TABSTRIP) ||
      browser->tabstrip_model()->empty();
}
