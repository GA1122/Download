void BackendIO::CloseEntryImpl(EntryImpl* entry) {
  operation_ = OP_CLOSE_ENTRY;
  entry_ = entry;
}
