ui::MenuModel* ExtensionAppItem::GetContextMenuModel() {
  context_menu_.reset(new app_list::AppContextMenu(
      this, profile_, extension_id_, GetController()));
  context_menu_->set_is_platform_app(is_platform_app_);
  if (IsInFolder())
    context_menu_->set_is_in_folder(true);
  return context_menu_->GetMenuModel();
}
