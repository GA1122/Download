void Browser::TabDeactivated(TabContentsWrapper* contents) {
  if (contents == fullscreened_tab_)
    ExitTabbedFullscreenModeIfNecessary();
  if (instant())
    instant()->DestroyPreviewContents();

  window_->GetLocationBar()->SaveStateToContents(contents->tab_contents());
}
