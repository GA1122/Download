TimeDelta SyncManager::SyncInternal::GetNudgeDelayTimeDelta(
    const ModelType& model_type) {
  return NudgeStrategy::GetNudgeDelayTimeDelta(model_type, this);
}
