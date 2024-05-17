void ConfigureEntriesForRestore(
    std::vector<linked_ptr<NavigationEntryImpl> >* entries,
    NavigationController::RestoreType type) {
  for (size_t i = 0; i < entries->size(); ++i) {
    (*entries)[i]->SetTransitionType(PAGE_TRANSITION_RELOAD);
    (*entries)[i]->set_restore_type(ControllerRestoreTypeToEntryType(type));
    SetPageStateIfEmpty((*entries)[i].get());
  }
}
