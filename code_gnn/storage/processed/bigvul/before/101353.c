bool SessionModelAssociator::SessionWindowHasNoTabsToSync(
    const SessionWindow& window) {
  int num_populated = 0;
  for (std::vector<SessionTab*>::const_iterator i = window.tabs.begin();
      i != window.tabs.end(); ++i) {
    const SessionTab* tab = *i;
    if (IsValidSessionTab(*tab))
      num_populated++;
  }
  if (num_populated == 0)
    return true;
  return false;
}
