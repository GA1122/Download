void BackendIO::ExecuteOperation() {
  if (IsEntryOperation())
    return ExecuteEntryOperation();

  ExecuteBackendOperation();
}
