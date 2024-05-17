void BackendIO::ReadyForSparseIO(EntryImpl* entry) {
  operation_ = OP_IS_READY;
  entry_ = entry;
}
