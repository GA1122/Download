void Shell::DismissAppList() {
  if (!app_list_controller_)
    return;
  app_list_controller_->Dismiss();
}
