void BrowserView::ShowDevToolsContainer() {
  if (!devtools_focus_tracker_.get()) {
    devtools_focus_tracker_.reset(
        new views::ExternalFocusTracker(devtools_container_,
                                        GetFocusManager()));
  }
  devtools_container_->SetVisible(true);
  devtools_dock_side_ = devtools_window_->dock_side();
  bool dock_to_right = devtools_dock_side_ == DEVTOOLS_DOCK_SIDE_RIGHT;
  contents_split_->set_orientation(
      dock_to_right ? views::SingleSplitView::HORIZONTAL_SPLIT
                    : views::SingleSplitView::VERTICAL_SPLIT);
  UpdateDevToolsSplitPosition();
  contents_split_->InvalidateLayout();
  Layout();
}
