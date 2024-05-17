void Browser::TabMoved(WebContents* contents,
                       int from_index,
                       int to_index) {
  DCHECK(from_index >= 0 && to_index >= 0);
  SyncHistoryWithTabs(std::min(from_index, to_index));
}
