void Browser::URLStarredChanged(TabContentsWrapper* source, bool starred) {
  if (source == GetSelectedTabContentsWrapper())
    window_->SetStarredState(starred);
}
