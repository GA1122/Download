void Browser::TabDeselected(TabContentsWrapper* contents) {
  if (instant())
    instant()->DestroyPreviewContents();

  window_->GetLocationBar()->SaveStateToContents(contents->tab_contents());
}
