void DevToolsWindow::SetIsDocked(bool dock_requested) {
  if (life_stage_ == kClosing)
    return;

  DCHECK(can_dock_ || !dock_requested);
  if (!can_dock_)
    dock_requested = false;

  bool was_docked = is_docked_;
  is_docked_ = dock_requested;

  if (life_stage_ != kLoadCompleted) {
    life_stage_ = life_stage_ == kOnLoadFired ? kLoadCompleted : kIsDockedSet;
    if (life_stage_ == kLoadCompleted)
      LoadCompleted();
    return;
  }

  if (dock_requested == was_docked)
    return;

  if (dock_requested && !was_docked) {
    TabStripModel* tab_strip_model = browser_->tab_strip_model();
    tab_strip_model->DetachWebContentsAt(
        tab_strip_model->GetIndexOfWebContents(main_web_contents_));
    browser_ = NULL;
  } else if (!dock_requested && was_docked) {
    UpdateBrowserWindow();
  }

  Show(DevToolsToggleAction::Show());
}
