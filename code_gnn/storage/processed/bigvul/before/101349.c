void SessionModelAssociator::ReassociateTabs(
    const std::vector<SyncedTabDelegate*>& tabs) {
  DCHECK(CalledOnValidThread());
  for (std::vector<SyncedTabDelegate*>::const_iterator i = tabs.begin();
       i != tabs.end();
       ++i) {
    ReassociateTab(**i);
  }
  if (waiting_for_change_) QuitLoopForSubtleTesting();
}
