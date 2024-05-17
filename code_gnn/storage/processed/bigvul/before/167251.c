FetchManager* FetchManager::Create(ExecutionContext* execution_context) {
  return new FetchManager(execution_context);
}
