DiscardableSharedMemoryManager::DiscardableSharedMemoryManager()
    : next_client_id_(1),
      default_memory_limit_(GetDefaultMemoryLimit()),
      memory_limit_(default_memory_limit_),
      bytes_allocated_(0),
      memory_pressure_listener_(new base::MemoryPressureListener(
          base::Bind(&DiscardableSharedMemoryManager::OnMemoryPressure,
                     base::Unretained(this)))),
      enforce_memory_policy_task_runner_(base::ThreadTaskRunnerHandle::Get()),
      enforce_memory_policy_pending_(false),
      mojo_thread_message_loop_(nullptr),
      weak_ptr_factory_(this),
      mojo_thread_weak_ptr_factory_(this) {
  DCHECK_NE(memory_limit_, 0u);
  enforce_memory_policy_callback_ =
      base::Bind(&DiscardableSharedMemoryManager::EnforceMemoryPolicy,
                 weak_ptr_factory_.GetWeakPtr());
  base::trace_event::MemoryDumpManager::GetInstance()->RegisterDumpProvider(
      this, "DiscardableSharedMemoryManager",
      base::ThreadTaskRunnerHandle::Get());
  base::MemoryCoordinatorClientRegistry::GetInstance()->Register(this);
}
