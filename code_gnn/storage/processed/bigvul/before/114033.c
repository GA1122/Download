void TaskManagerView::ActivateFocusedTab() {
  int row_count = tab_table_->RowCount();
  for (int i = 0; i < row_count; ++i) {
    if (tab_table_->ItemHasTheFocus(i)) {
      task_manager_->ActivateProcess(i);
      break;
    }
  }
}
