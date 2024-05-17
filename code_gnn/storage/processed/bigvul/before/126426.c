void BrowserWindowGtk::Show() {
  BrowserList::SetLastActive(browser());

  gtk_window_present(window_);
  if (show_state_after_show_ == ui::SHOW_STATE_MAXIMIZED) {
    gtk_window_maximize(window_);
    show_state_after_show_ = ui::SHOW_STATE_NORMAL;
  } else if (show_state_after_show_ == ui::SHOW_STATE_MINIMIZED) {
    gtk_window_iconify(window_);
    show_state_after_show_ = ui::SHOW_STATE_NORMAL;
  }

  gtk_widget_set_size_request(contents_container_->widget(), -1, -1);

  window_has_shown_ = true;
  browser()->OnWindowDidShow();
  if (devtools_window_)
    UpdateDevToolsSplitPosition();
}
