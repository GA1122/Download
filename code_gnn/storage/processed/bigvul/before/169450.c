void IncrementBytesSent(uint64_t bytes) {
  NetworkActivityMonitor::GetInstance()->IncrementBytesSent(bytes);
}
