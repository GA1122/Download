bool ScriptResource::CanUseCacheValidator() const {
  if (HasClientsOrObservers())
    return false;

  return Resource::CanUseCacheValidator();
}
