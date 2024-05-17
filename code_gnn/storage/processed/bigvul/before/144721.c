 TaskService::~TaskService() {
   std::vector<std::unique_ptr<base::Thread>> threads;
  {
    base::AutoLock lock(lock_);
    threads = std::move(threads_);
    DCHECK_EQ(kInvalidInstanceId, bound_instance_id_);
  }
  threads.clear();
}
