void TaskManagerView::OnSelectionChanged() {
  bool selection_contains_browser_process = false;
  for (views::TableSelectionIterator iter  = tab_table_->SelectionBegin();
       iter != tab_table_->SelectionEnd(); ++iter) {
    if (task_manager_->IsBrowserProcess(*iter)) {
      selection_contains_browser_process = true;
      break;
    }
  }
  kill_button_->SetEnabled(!selection_contains_browser_process &&
                           tab_table_->SelectedRowCount() > 0);
}
