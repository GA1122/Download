ClientDiscardableSharedMemoryManager::ClientDiscardableSharedMemoryManager(
    mojom::DiscardableSharedMemoryManagerPtr manager,
    scoped_refptr<base::SingleThreadTaskRunner> io_task_runner)
    : io_task_runner_(std::move(io_task_runner)),
      manager_mojo_(new mojom::DiscardableSharedMemoryManagerPtr),
      heap_(new DiscardableSharedMemoryHeap(base::GetPageSize())) {
  base::trace_event::MemoryDumpManager::GetInstance()->RegisterDumpProvider(
      this, "ClientDiscardableSharedMemoryManager",
      base::ThreadTaskRunnerHandle::Get());
  mojom::DiscardableSharedMemoryManagerPtrInfo info = manager.PassInterface();
  io_task_runner_->PostTask(
      FROM_HERE, base::Bind(&InitManagerMojoOnIO, manager_mojo_.get(),
                            base::Passed(&info)));
}
