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

  GURL url = preview_contents->tab_contents()->GetURL();
  if (profile_->GetExtensionService()->IsInstalledApp(url)) {
    UMA_HISTOGRAM_ENUMERATION(extension_misc::kAppLaunchHistogram,
                              extension_misc::APP_LAUNCH_OMNIBOX_INSTANT,
                              extension_misc::APP_LAUNCH_BUCKET_BOUNDARY);
  }
}
