JingleSessionManager::JingleSessionManager(
    base::MessageLoopProxy* message_loop)
    : message_loop_(message_loop),
      signal_strategy_(NULL),
      allow_nat_traversal_(false),
      allow_local_ips_(false),
      http_port_allocator_(NULL),
      closed_(false),
      ALLOW_THIS_IN_INITIALIZER_LIST(task_factory_(this)) {
 }
