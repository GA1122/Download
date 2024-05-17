void IncrementBytesReceived(uint64_t bytes) {
  NetworkActivityMonitor::GetInstance()->IncrementBytesReceived(bytes);
}
