void BrowserView::UpdateDevToolsSplitPosition() {
  if (devtools_window_->dock_side() == DEVTOOLS_DOCK_SIDE_RIGHT) {
    int split_offset = contents_split_->width() -
        devtools_window_->GetWidth(contents_split_->width());
    contents_split_->set_divider_offset(split_offset);
  } else {
    int split_offset = contents_split_->height() -
        devtools_window_->GetHeight(contents_split_->height());
    contents_split_->set_divider_offset(split_offset);
  }
}
