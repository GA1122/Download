CurrentHistoryCleaner::CurrentHistoryCleaner(content::WebContents* contents)
    : WebContentsObserver(contents) {
  content::NavigationController& nc = web_contents()->GetController();
  history_index_to_remove_ = nc.GetLastCommittedEntryIndex();
}
