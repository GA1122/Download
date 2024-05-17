void Browser::DetachContents(TabContents* source) {
  int index = tab_handler_->GetTabStripModel()->GetWrapperIndex(source);
  if (index >= 0)
    tab_handler_->GetTabStripModel()->DetachTabContentsAt(index);
}
