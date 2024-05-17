void NonWhitelistedCommandsAreDisabled(CommandUpdaterImpl* command_updater) {
  constexpr int kWhitelistedIds[] = {
    IDC_CUT, IDC_COPY, IDC_PASTE,
    IDC_FIND, IDC_FIND_NEXT, IDC_FIND_PREVIOUS,
    IDC_ZOOM_PLUS, IDC_ZOOM_NORMAL, IDC_ZOOM_MINUS,
  };

  for (int id : command_updater->GetAllIds()) {
    if (base::ContainsValue(kWhitelistedIds, id)) {
      continue;
    }
    DCHECK(!command_updater->IsCommandEnabled(id));
  }
}
