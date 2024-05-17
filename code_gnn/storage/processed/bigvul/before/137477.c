RunLoop::Delegate::Delegate() {
  DETACH_FROM_THREAD(bound_thread_checker_);
}
