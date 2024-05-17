void Browser::ActivateContents(TabContents* contents) {
  tab_handler_->GetTabStripModel()->SelectTabContentsAt(
      tab_handler_->GetTabStripModel()->GetWrapperIndex(contents), false);
  window_->Activate();
}
