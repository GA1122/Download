void HostCache::RecordEraseAll(EraseReason reason, base::TimeTicks now) {
  for (const auto& it : entries_)
    RecordErase(reason, now, it.second);
}
