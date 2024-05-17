  base::TimeTicks Now() {
    if (now_.is_null())
      now_ = task_queue_manager_->Now();
    return now_;
  }
