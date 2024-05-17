  bool ShouldSwapProcesses(RenderViewHostManager* manager,
                           const NavigationEntryImpl* cur_entry,
                           const NavigationEntryImpl* new_entry) const {
    return manager->ShouldSwapProcessesForNavigation(cur_entry, new_entry);
  }
