void FetchManager::ContextDestroyed(ExecutionContext*) {
  for (auto& loader : loaders_)
    loader->Dispose();
}
