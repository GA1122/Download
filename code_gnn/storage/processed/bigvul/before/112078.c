  static TimeDelta GetNudgeDelayTimeDelta(const ModelType& model_type,
                                          SyncManager::SyncInternal* core) {
    NudgeDelayStrategy delay_type = GetNudgeDelayStrategy(model_type);
    return GetNudgeDelayTimeDeltaFromType(delay_type,
                                          model_type,
                                          core);
  }
