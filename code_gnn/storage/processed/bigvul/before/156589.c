std::unique_ptr<syncer::DataBatch> SessionStore::GetSessionDataForKeys(
    const std::vector<std::string>& storage_keys) const {
  std::map<std::string, std::set<int>> session_tag_to_node_ids;
  for (const std::string& storage_key : storage_keys) {
    std::string session_tag;
    int tab_node_id;
    bool success = DecodeStorageKey(storage_key, &session_tag, &tab_node_id);
    DCHECK(success);
    session_tag_to_node_ids[session_tag].insert(tab_node_id);
  }
  auto batch = std::make_unique<syncer::MutableDataBatch>();
  SerializePartialTrackerToSpecifics(
      session_tracker_, session_tag_to_node_ids,
      base::BindRepeating(
          [](syncer::MutableDataBatch* batch, const std::string& session_name,
             sync_pb::SessionSpecifics* specifics) {
            DCHECK(AreValidSpecifics(*specifics));
            const std::string storage_key = GetStorageKey(*specifics);
            batch->Put(storage_key, MoveToEntityData(session_name, specifics));
          },
          batch.get()));
  return batch;
}
