void Browser::CloseContents(TabContents* source) {
  if (is_attempting_to_close_browser_) {
    ClearUnloadState(source, true);
    return;
  }

  TabContentsWrapper* source_wrapper =
      TabContentsWrapper::GetCurrentWrapperForContents(source);
  if (g_browser_process->background_printing_manager()->
          OwnInitiatorTab(source_wrapper)) {
    return;
  }

  int index = tab_handler_->GetTabStripModel()->GetWrapperIndex(source);
  if (index == TabStripModel::kNoTab) {
    NOTREACHED() << "CloseContents called for tab not in our strip";
    return;
  }
  tab_handler_->GetTabStripModel()->CloseTabContentsAt(
      index,
      TabStripModel::CLOSE_CREATE_HISTORICAL_TAB);
}
