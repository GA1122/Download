   void Erase(const std::string& addr) {
//    
//   void Erase(int32 preview_id) {
      base::AutoLock lock(lock_);
    map_.erase(addr);
//     map_.erase(preview_id);
    }