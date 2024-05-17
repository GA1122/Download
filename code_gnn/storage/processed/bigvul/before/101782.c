void Browser::ReloadInternal(WindowOpenDisposition disposition,
                             bool ignore_cache) {
  TabContents* current_tab = GetSelectedTabContents();
  if (current_tab && current_tab->showing_interstitial_page()) {
    NavigationEntry* entry = current_tab->controller().GetActiveEntry();
    DCHECK(entry);   
    OpenURL(entry->url(), GURL(), disposition, PageTransition::RELOAD);
    return;
  }

  TabContents* tab = GetOrCloneTabForDisposition(disposition);
  if (!tab->FocusLocationBarByDefault())
    tab->Focus();
  if (ignore_cache)
    tab->controller().ReloadIgnoringCache(true);
  else
    tab->controller().Reload(true);
}
