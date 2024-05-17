 HostCache::~HostCache() {
   DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  RecordEraseAll(ERASE_DESTRUCT, base::TimeTicks::Now());
}
