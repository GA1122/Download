void ThreadHeapStats::Reset() {
  object_size_at_last_gc_ = allocated_object_size_ + marked_object_size_;
  partition_alloc_size_at_last_gc_ =
      WTF::Partitions::TotalSizeOfCommittedPages();
  allocated_object_size_ = 0;
  marked_object_size_ = 0;
  wrapper_count_at_last_gc_ = wrapper_count_;
  collected_wrapper_count_ = 0;
}
