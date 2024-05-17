  void set_queue_pointer(const ThrottleListQueuePointer& pointer) {
    if (state_ != State::AGED)
      DCHECK_EQ(this, *pointer);
    queue_pointer_ = pointer;
  }
