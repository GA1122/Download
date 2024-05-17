QuicStreamHost::QuicStreamHost() {
  DETACH_FROM_THREAD(thread_checker_);
}
