void Browser::TabDeselectedAt(TabContentsWrapper* contents, int index) {
  if (instant())
    instant()->DestroyPreviewContents();

  window_->GetLocationBar()->SaveStateToContents(contents->tab_contents());
}
