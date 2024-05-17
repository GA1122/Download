std::string GetModelContent(app_list::AppListModel* model) {
  std::string content;
  for (size_t i = 0; i < model->top_level_item_list()->item_count(); ++i) {
    if (i > 0)
      content += ',';
    content += model->top_level_item_list()->item_at(i)->name();
  }
  return content;
}
