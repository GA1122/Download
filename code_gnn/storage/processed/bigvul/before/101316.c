bool SessionModelAssociator::AssociateModels(SyncError* error) {
  DCHECK(CalledOnValidThread());

  DCHECK(synced_session_tracker_.empty());
  DCHECK_EQ(0U, tab_pool_.capacity());

  local_session_syncid_ = sync_api::kInvalidId;

  {
    sync_api::WriteTransaction trans(FROM_HERE, sync_service_->GetUserShare());

    sync_api::ReadNode root(&trans);
    if (!root.InitByTagLookup(kSessionsTag)) {
      error->Reset(FROM_HERE, kNoSessionsFolderError, model_type());
      return false;
    }

    if (current_machine_tag_.empty()) {
      InitializeCurrentMachineTag(&trans);
      InitializeCurrentSessionName();
    }
    synced_session_tracker_.SetLocalSessionTag(current_machine_tag_);
    if (!UpdateAssociationsFromSyncModel(root, &trans)) {
      error->Reset(FROM_HERE,
                   "Failed to update associations from sync",
                   model_type());
      return false;
    }

    if (local_session_syncid_ == sync_api::kInvalidId) {
      sync_api::WriteNode write_node(&trans);
      if (!write_node.InitUniqueByCreation(syncable::SESSIONS, root,
          current_machine_tag_)) {
        error->Reset(FROM_HERE,
                     "Failed to create sessions header sync node.",
                     model_type());
        return false;
      }
      write_node.SetTitle(UTF8ToWide(current_machine_tag_));
      local_session_syncid_ = write_node.GetId();
    }
  }

  UpdateSyncModelDataFromClient();

  VLOG(1) << "Session models associated.";

  return true;
}
