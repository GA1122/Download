void CurrentHistoryCleaner::DidStopLoading(
    content::RenderViewHost* render_view_host) {
  content::NavigationController& nc = web_contents()->GetController();
  if (history_index_to_remove_ < nc.GetLastCommittedEntryIndex()) {
    nc.RemoveEntryAtIndex(history_index_to_remove_);
    delete this;   
  }
}
