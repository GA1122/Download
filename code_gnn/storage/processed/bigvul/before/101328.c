int64 SessionModelAssociator::TabNodePool::GetFreeTabNode() {
  DCHECK_GT(machine_tag_.length(), 0U);
  if (tab_pool_fp_ == -1) {
    sync_api::WriteTransaction trans(FROM_HERE, sync_service_->GetUserShare());
    sync_api::ReadNode root(&trans);
    if (!root.InitByTagLookup(kSessionsTag)) {
      LOG(ERROR) << kNoSessionsFolderError;
      return sync_api::kInvalidId;
    }
    size_t tab_node_id = tab_syncid_pool_.size();
    std::string tab_node_tag = TabIdToTag(machine_tag_, tab_node_id);
    sync_api::WriteNode tab_node(&trans);
    if (!tab_node.InitUniqueByCreation(syncable::SESSIONS, root,
                                       tab_node_tag)) {
      LOG(ERROR) << "Could not create new node with tag "
                 << tab_node_tag << "!";
      return sync_api::kInvalidId;
    }
    tab_node.SetTitle(UTF8ToWide(tab_node_tag));

    tab_syncid_pool_.resize(tab_node_id + 1);
    VLOG(1) << "Adding sync node " << tab_node.GetId() << " to tab syncid pool";
    return tab_node.GetId();
  } else {
    return tab_syncid_pool_[static_cast<size_t>(tab_pool_fp_--)];
  }
}
