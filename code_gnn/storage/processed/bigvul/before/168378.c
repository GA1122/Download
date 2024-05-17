void BrowserView::UpdateDevToolsForContents(
    WebContents* web_contents, bool update_devtools_web_contents) {
  DevToolsContentsResizingStrategy strategy;
  WebContents* devtools = DevToolsWindow::GetInTabWebContents(
      web_contents, &strategy);

  if (!devtools_web_view_->web_contents() && devtools &&
      !devtools_focus_tracker_.get()) {
    devtools_focus_tracker_.reset(
        new views::ExternalFocusTracker(devtools_web_view_,
                                        GetFocusManager()));
  }

  if (devtools_web_view_->web_contents() && !devtools &&
      devtools_focus_tracker_.get()) {
    devtools_focus_tracker_->FocusLastFocusedExternalView();
    devtools_focus_tracker_.reset();
  }

  if (devtools_web_view_->web_contents() != devtools &&
      update_devtools_web_contents) {
    devtools_web_view_->SetWebContents(devtools);
  }

  if (devtools) {
    devtools_web_view_->SetVisible(true);
    GetContentsLayoutManager()->SetContentsResizingStrategy(strategy);
  } else {
    devtools_web_view_->SetVisible(false);
    GetContentsLayoutManager()->SetContentsResizingStrategy(
        DevToolsContentsResizingStrategy());
  }
  contents_container_->Layout();

  if (devtools) {
    int devtools_index = contents_container_->GetIndexOf(devtools_web_view_);
    int contents_index = contents_container_->GetIndexOf(contents_web_view_);
    bool devtools_is_on_top = devtools_index > contents_index;
    if (strategy.hide_inspected_contents() != devtools_is_on_top)
      contents_container_->ReorderChildView(contents_web_view_, devtools_index);
  }
}
