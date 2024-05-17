  explicit LazyNow(base::TimeTicks now)
      : task_queue_manager_(nullptr), now_(now) {
    DCHECK(!now.is_null());
  }
