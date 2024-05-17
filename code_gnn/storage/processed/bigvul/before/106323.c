ChangeProcessor* SyncBackendHost::Core::GetProcessor(
    syncable::ModelType model_type) {
  std::map<syncable::ModelType, ChangeProcessor*>::const_iterator it =
      host_->processors_.find(model_type);

  if (it == host_->processors_.end())
    return NULL;

  if (!IsCurrentThreadSafeForModel(model_type)) {
    NOTREACHED() << "Changes applied on wrong thread.";
    return NULL;
  }

  ChangeProcessor* processor = it->second;

  if (!processor->IsRunning())
    return NULL;

  return processor;
}
