HFSForkReadStream::HFSForkReadStream(HFSIterator* hfs,
                                     const HFSPlusForkData& fork)
    : hfs_(hfs),
      fork_(fork),
      current_extent_(0),
      read_current_extent_(false),
      current_extent_data_(),
      fork_logical_offset_(0) {
}
