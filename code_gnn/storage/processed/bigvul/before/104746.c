void NavigationController::CopyStateFrom(const NavigationController& source) {
  DCHECK(entry_count() == 0 && !pending_entry());

  if (source.entry_count() == 0)
    return;   

  needs_reload_ = true;
  InsertEntriesFrom(source, source.entry_count());

  session_storage_namespace_ = source.session_storage_namespace_->Clone();

  FinishRestore(source.last_committed_entry_index_, false);
}
