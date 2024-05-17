  ui::MenuModel* GetContextMenuModel() {
    test_api_->UpdateContextMenu();
    return test_api_->context_menu_contents();
  }
