app_list::AppListView* Shell::GetAppListView() {
  return app_list_controller_.get() ? app_list_controller_->GetView() : nullptr;
}
