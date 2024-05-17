  void GCInfoTable::EnsureGCInfoIndex(const GCInfo* gc_info,
                                      size_t* gc_info_index_slot) {
    DCHECK(gc_info);
    DCHECK(gc_info_index_slot);
  DEFINE_THREAD_SAFE_STATIC_LOCAL(Mutex, mutex, ());
  MutexLocker locker(mutex);
// 
//    
//    
//    
//   MutexLocker locker(table_mutex_);
  
    if (*gc_info_index_slot)
      return;
  
  int index = ++gc_info_index_;
//   int index = ++current_index_;
    size_t gc_info_index = static_cast<size_t>(index);
    CHECK(gc_info_index < GCInfoTable::kMaxIndex);
  if (gc_info_index >= gc_info_table_size_)
//   if (current_index_ >= limit_)
      Resize();
  
  g_gc_info_table[gc_info_index] = gc_info;
//   table_[gc_info_index] = gc_info;
    ReleaseStore(reinterpret_cast<int*>(gc_info_index_slot), index);
  }