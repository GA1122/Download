void SessionModelAssociator::OnGotSession(
    int handle,
    std::vector<SessionWindow*>* windows) {
  DCHECK(CalledOnValidThread());
  DCHECK(local_session_syncid_);

  sync_pb::SessionSpecifics specifics;
  specifics.set_session_tag(GetCurrentMachineTag());
  sync_pb::SessionHeader* header_s = specifics.mutable_header();
  PopulateSessionSpecificsHeader(*windows, header_s);

  sync_api::WriteTransaction trans(FROM_HERE, sync_service_->GetUserShare());
  sync_api::ReadNode root(&trans);
  if (!root.InitByTagLookup(kSessionsTag)) {
    LOG(ERROR) << kNoSessionsFolderError;
    return;
  }

  sync_api::WriteNode header_node(&trans);
  if (!header_node.InitByIdLookup(local_session_syncid_)) {
    LOG(ERROR) << "Failed to load local session header node.";
    return;
  }

  header_node.SetSessionSpecifics(specifics);
}
