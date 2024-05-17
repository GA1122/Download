std::unique_ptr<syncer::DataBatch> SessionStore::GetAllSessionData() const {
  auto batch = std::make_unique<syncer::MutableDataBatch>();
  SerializeTrackerToSpecifics(
      session_tracker_,
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
