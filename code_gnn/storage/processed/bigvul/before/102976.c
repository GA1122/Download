void TabStripModel::InternalCloseTab(TabContentsWrapper* contents,
                                     int index,
                                     bool create_historical_tabs) {
  FOR_EACH_OBSERVER(TabStripModelObserver, observers_,
                    TabClosingAt(this, contents, index));

  if (create_historical_tabs)
    delegate_->CreateHistoricalTab(contents);

  delete contents;
}
