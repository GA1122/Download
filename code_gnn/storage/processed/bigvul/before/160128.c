void BackendIO::DoomEntryImpl(EntryImpl* entry) {
  operation_ = OP_DOOM_ENTRY;
  entry_ = entry;
}
