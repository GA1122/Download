void NavigationController::FinishRestore(int selected_index) {
  DCHECK(selected_index >= 0 && selected_index < entry_count());
  ConfigureEntriesForRestore(&entries_);

  set_max_restored_page_id(entry_count());

  last_committed_entry_index_ = selected_index;
}
