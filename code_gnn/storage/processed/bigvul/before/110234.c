void NaClIPCAdapter::CloseChannelOnIOThread() {
  io_thread_data_.channel_->Close();
}
