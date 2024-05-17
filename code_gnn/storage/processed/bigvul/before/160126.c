void BackendIO::DoomEntry(const std::string& key) {
  operation_ = OP_DOOM;
  key_ = key;
}
