bool BackendIO::IsEntryOperation() {
  return operation_ > OP_MAX_BACKEND;
}
