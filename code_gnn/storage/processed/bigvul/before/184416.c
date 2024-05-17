   void Set(const std::string& addr, int value) {
//    
//   void Set(int32 preview_id, int value) {
      base::AutoLock lock(lock_);
    map_[addr] = value;
//     map_[preview_id] = value;
    }