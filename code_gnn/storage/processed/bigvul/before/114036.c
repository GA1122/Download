void TaskManagerView::ButtonPressed(
    views::Button* sender, const views::Event& event) {
  if (purge_memory_button_ && (sender == purge_memory_button_)) {
    MemoryPurger::PurgeAll();
  } else {
    DCHECK_EQ(kill_button_, sender);
    for (views::TableSelectionIterator iter  = tab_table_->SelectionBegin();
         iter != tab_table_->SelectionEnd(); ++iter)
      task_manager_->KillProcess(*iter);
  }
}
