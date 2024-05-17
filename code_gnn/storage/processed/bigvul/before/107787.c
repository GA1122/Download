void Browser::CommitInstant(TabContentsWrapper* preview_contents) {
  TabContentsWrapper* tab_contents = instant_->tab_contents();
  int index =
      tab_handler_->GetTabStripModel()->GetIndexOfTabContents(tab_contents);
  DCHECK_NE(TabStripModel::kNoTab, index);
  preview_contents->controller().CopyStateFromAndPrune(
      &tab_contents->controller());
  TabContentsWrapper* old_contents =
      tab_handler_->GetTabStripModel()->ReplaceTabContentsAt(
          index, preview_contents);
  instant_unload_handler_->RunUnloadListenersOrDestroy(old_contents, index);
}
