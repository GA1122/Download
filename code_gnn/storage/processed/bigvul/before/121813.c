void UDPSocketWin::Core::WatchForRead() {
  AddRef();
  read_watcher_.StartWatching(read_overlapped_.hEvent, &reader_);
}
