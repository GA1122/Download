void NavigationController::Restore(
    int selected_navigation,
    bool from_last_session,
    std::vector<NavigationEntry*>* entries) {
  DCHECK(entry_count() == 0 && !pending_entry());
  DCHECK(selected_navigation >= 0 &&
         selected_navigation < static_cast<int>(entries->size()));

  needs_reload_ = true;
  for (size_t i = 0; i < entries->size(); ++i)
    entries_.push_back(linked_ptr<NavigationEntry>((*entries)[i]));
  entries->clear();

  FinishRestore(selected_navigation, from_last_session);
}
