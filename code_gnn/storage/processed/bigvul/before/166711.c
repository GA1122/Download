ThreadHeapStats::ThreadHeapStats()
    : allocated_space_(0),
      allocated_object_size_(0),
      object_size_at_last_gc_(0),
      marked_object_size_(0),
      marked_object_size_at_last_complete_sweep_(0),
      wrapper_count_(0),
      wrapper_count_at_last_gc_(0),
      collected_wrapper_count_(0),
      partition_alloc_size_at_last_gc_(
          WTF::Partitions::TotalSizeOfCommittedPages()),
      estimated_marking_time_per_byte_(0.0) {}
