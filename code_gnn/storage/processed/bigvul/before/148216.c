void CursorImpl::IDBThreadHelper::PrefetchReset(int32_t used_prefetches,
                                                int32_t unused_prefetches) {
  leveldb::Status s =
      cursor_->PrefetchReset(used_prefetches, unused_prefetches);
  if (!s.ok())
    DLOG(ERROR) << "Unable to reset prefetch";
}
