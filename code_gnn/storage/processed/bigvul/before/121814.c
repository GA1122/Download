void UDPSocketWin::Core::WatchForWrite() {
  AddRef();
  write_watcher_.StartWatching(write_overlapped_.hEvent, &writer_);
}
