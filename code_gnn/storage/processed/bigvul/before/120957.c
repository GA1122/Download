size_t SocketStream::GetTotalSizeOfPendingWriteBufs() const {
  size_t total_size = 0;
  for (PendingDataQueue::const_iterator iter = pending_write_bufs_.begin();
       iter != pending_write_bufs_.end();
       ++iter)
    total_size += (*iter)->size();
  return total_size;
}
