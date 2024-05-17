  SyncSession* MakeSession() {
    ModelSafeRoutingInfo info;
    std::vector<ModelSafeWorker*> workers;
    GetModelSafeRoutingInfo(&info);
    GetWorkers(&workers);
    syncable::ModelTypePayloadMap types =
        syncable::ModelTypePayloadMapFromRoutingInfo(info, std::string());
    return new SyncSession(context_.get(), this,
        sessions::SyncSourceInfo(sync_pb::GetUpdatesCallerInfo::UNKNOWN, types),
        info, workers);
  }
