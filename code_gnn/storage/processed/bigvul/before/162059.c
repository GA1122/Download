  ConnectionFilterImpl(
      const service_manager::Identity& child_identity,
      std::unique_ptr<service_manager::BinderRegistry> registry)
      : child_identity_(child_identity),
        registry_(std::move(registry)),
        controller_(new ConnectionFilterController(this)),
        weak_factory_(this) {
    thread_checker_.DetachFromThread();
  }
