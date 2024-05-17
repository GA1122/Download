void TabStripModel::InternalCloseTab(WebContents* contents,
                                     int index,
                                     bool create_historical_tabs) {
  FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
                    TabClosingAt(this, contents, index));

  if (create_historical_tabs)
    delegate_->CreateHistoricalTab(contents);

  TabContents* tab_contents = TabContents::FromWebContents(contents);
  DCHECK(tab_contents);
  delete tab_contents;
}
