void LauncherView::GetOverflowItems(std::vector<LauncherItem>* items) {
  int index = 0;
  while (index < view_model_->view_size() &&
         view_model_->view_at(index)->visible()) {
    index++;
  }
  while (index < view_model_->view_size()) {
    const LauncherItem& item = model_->items()[index];
    if (item.type == TYPE_TABBED ||
        item.type == TYPE_APP_PANEL ||
        item.type == TYPE_APP_SHORTCUT)
      items->push_back(item);
    index++;
   }
 }
