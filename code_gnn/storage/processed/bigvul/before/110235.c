void NaClIPCAdapter::ConnectChannelOnIOThread() {
  if (!io_thread_data_.channel_->Connect())
    NOTREACHED();
}
