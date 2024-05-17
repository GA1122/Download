void BackendIO::DoomEntriesSince(const base::Time initial_time) {
  operation_ = OP_DOOM_SINCE;
  initial_time_ = initial_time;
}
