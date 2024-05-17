   bool Get(const std::string& addr, int* out_value) {
//    
//    
//   bool Get(int32 preview_id, int* out_value) {
      base::AutoLock lock(lock_);
    PrintPreviewRequestIdMap::const_iterator it = map_.find(addr);
//     PrintPreviewRequestIdMap::const_iterator it = map_.find(preview_id);
      if (it == map_.end())
        return false;
      *out_value = it->second;
      return true;
    }