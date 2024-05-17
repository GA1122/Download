ShelfDelegate* Shell::GetShelfDelegate() {
  if (!shelf_delegate_) {
    shelf_model_.reset(new ShelfModel);
    shelf_item_delegate_manager_.reset(
        new ShelfItemDelegateManager(shelf_model_.get()));

    shelf_delegate_.reset(delegate_->CreateShelfDelegate(shelf_model_.get()));
    scoped_ptr<ShelfItemDelegate> controller(new AppListShelfItemDelegate);

    int app_list_index = shelf_model_->GetItemIndexForType(TYPE_APP_LIST);
    DCHECK_GE(app_list_index, 0);
    ShelfID app_list_id = shelf_model_->items()[app_list_index].id;
    DCHECK(app_list_id);
    shelf_item_delegate_manager_->SetShelfItemDelegate(app_list_id,
                                                       std::move(controller));
    shelf_window_watcher_.reset(new ShelfWindowWatcher(
        shelf_model_.get(), shelf_item_delegate_manager_.get()));
  }
  return shelf_delegate_.get();
}
