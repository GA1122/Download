ClientDiscardableSharedMemoryManager::~ClientDiscardableSharedMemoryManager() {
  base::trace_event::MemoryDumpManager::GetInstance()->UnregisterDumpProvider(
      this);
  if (heap_->GetSize())
    MemoryUsageChanged(0, 0);

  heap_.reset();

  bool posted = io_task_runner_->DeleteSoon(FROM_HERE, manager_mojo_.release());
  if (!posted)
    manager_mojo_.reset();
}
