void Browser::SwapTabContents(TabContentsWrapper* old_tab_contents,
                              TabContentsWrapper* new_tab_contents) {
  int index =
      tab_handler_->GetTabStripModel()->GetIndexOfTabContents(old_tab_contents);
  DCHECK_NE(TabStripModel::kNoTab, index);
  tab_handler_->GetTabStripModel()->ReplaceTabContentsAt(index,
                                                         new_tab_contents);
}
