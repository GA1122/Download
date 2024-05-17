bool SessionModelAssociator::WriteSessionTabToSyncModel(
    const SessionTab& tab,
    const int64 sync_id,
    sync_api::WriteTransaction* trans) {
  DCHECK(CalledOnValidThread());
  sync_api::WriteNode tab_node(trans);
  if (!tab_node.InitByIdLookup(sync_id)) {
    LOG(ERROR) << "Failed to look up tab node " << sync_id;
    return false;
  }

  sync_pb::SessionSpecifics specifics;
  specifics.set_session_tag(GetCurrentMachineTag());
  sync_pb::SessionTab* tab_s = specifics.mutable_tab();
  PopulateSessionSpecificsTab(tab, tab_s);
  tab_node.SetSessionSpecifics(specifics);
  return true;
}
