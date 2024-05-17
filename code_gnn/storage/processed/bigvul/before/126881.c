void BrowserView::UpdateDevToolsForContents(TabContents* tab_contents) {
  DevToolsWindow* new_devtools_window = tab_contents ?
      DevToolsWindow::GetDockedInstanceForInspectedTab(
          tab_contents->web_contents()) : NULL;
  if (devtools_window_ == new_devtools_window) {
    if (!new_devtools_window ||
        (new_devtools_window->dock_side() == devtools_dock_side_)) {
      return;
    }
  }

  if (devtools_window_ != new_devtools_window) {
    devtools_container_->SetWebContents(new_devtools_window ?
        new_devtools_window->tab_contents()->web_contents() : NULL);
  }

  if (devtools_window_) {
    if (devtools_dock_side_ == DEVTOOLS_DOCK_SIDE_RIGHT) {
      devtools_window_->SetWidth(
          contents_split_->width() - contents_split_->divider_offset());
    } else {
      devtools_window_->SetHeight(
          contents_split_->height() - contents_split_->divider_offset());
    }
  }

  bool should_hide = devtools_window_ && (!new_devtools_window ||
      devtools_dock_side_ != new_devtools_window->dock_side());
  bool should_show = new_devtools_window && (!devtools_window_ || should_hide);

  if (should_hide)
    HideDevToolsContainer();

  devtools_window_ = new_devtools_window;

  if (should_show) {
    devtools_dock_side_ = new_devtools_window->dock_side();
    ShowDevToolsContainer();
  } else if (new_devtools_window) {
    UpdateDevToolsSplitPosition();
  }
}
