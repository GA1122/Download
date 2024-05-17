void LoginDisplayHostWebUI::ResetLoginWindowAndView() {
  if (login_view_) {
    login_view_->SetUIEnabled(true);
    login_view_ = nullptr;
  }

  if (login_window_) {
    if (!features::IsAshInBrowserProcess()) {
      login_window_->Close();
    } else {
      login_window_->Hide();
      new CloseAfterCommit(login_window_);
    }
    login_window_->RemoveRemovalsObserver(this);
    login_window_ = nullptr;
    login_window_delegate_ = nullptr;
  }

  wizard_controller_.reset();
}
