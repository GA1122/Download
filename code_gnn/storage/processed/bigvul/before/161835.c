ClientDiscardableSharedMemoryManager::AllocateLockedDiscardableSharedMemory(
    size_t size,
    int32_t id) {
  TRACE_EVENT2("renderer",
               "ClientDiscardableSharedMemoryManager::"
               "AllocateLockedDiscardableSharedMemory",
               "size", size, "id", id);
  base::SharedMemoryHandle handle;
  base::WaitableEvent event(base::WaitableEvent::ResetPolicy::MANUAL,
                            base::WaitableEvent::InitialState::NOT_SIGNALED);
  base::ScopedClosureRunner event_signal_runner(
      base::Bind(&base::WaitableEvent::Signal, base::Unretained(&event)));
  io_task_runner_->PostTask(
      FROM_HERE, base::Bind(&ClientDiscardableSharedMemoryManager::AllocateOnIO,
                            base::Unretained(this), size, id, &handle,
                            base::Passed(&event_signal_runner)));
  event.Wait();
  auto memory = base::MakeUnique<base::DiscardableSharedMemory>(handle);
  if (!memory->Map(size))
    base::TerminateBecauseOutOfMemory(size);
  return memory;
}
