ui::MenuModel* AppResult::GetContextMenuModel() {
  if (!context_menu_) {
    context_menu_.reset(new AppContextMenu(
        this, profile_, app_id_, controller_));
    context_menu_->set_is_platform_app(is_platform_app_);
    context_menu_->set_is_search_result(true);
  }

  return context_menu_->GetMenuModel();
}
