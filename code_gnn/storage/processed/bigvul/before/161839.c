void ClientDiscardableSharedMemoryManager::DeletedDiscardableSharedMemory(
    int32_t id) {
  io_task_runner_->PostTask(
      FROM_HERE,
      base::Bind(&DeletedDiscardableSharedMemoryOnIO, manager_mojo_.get(), id));
}
