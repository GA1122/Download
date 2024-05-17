void SessionModelAssociator::PopulateSessionSpecificsHeader(
    const std::vector<SessionWindow*>& windows,
    sync_pb::SessionHeader* header_s) {
  DCHECK(CalledOnValidThread());

  for (size_t i = 0; i < windows.size(); ++i) {
    if (SessionWindowHasNoTabsToSync(*(windows[i])))
      continue;
    sync_pb::SessionWindow* window_s = header_s->add_window();
    PopulateSessionSpecificsWindow(*(windows[i]), window_s);
    if (!SyncLocalWindowToSyncModel(*(windows[i])))
      return;
  }
}
