void ConfigureEntriesForRestore(
    std::vector<linked_ptr<NavigationEntry> >* entries) {
  for (size_t i = 0; i < entries->size(); ++i) {
    (*entries)[i]->set_transition_type(PageTransition::RELOAD);
    (*entries)[i]->set_restored(true);
    SetContentStateIfEmpty((*entries)[i].get());
  }
}
