void SpdyProxyClientSocket::OnDataReceived(scoped_ptr<SpdyBuffer> buffer) {
  if (buffer) {
    net_log_.AddByteTransferEvent(NetLog::TYPE_SOCKET_BYTES_RECEIVED,
                                  buffer->GetRemainingSize(),
                                  buffer->GetRemainingData());
    read_buffer_queue_.Enqueue(buffer.Pass());
  } else {
    net_log_.AddByteTransferEvent(NetLog::TYPE_SOCKET_BYTES_RECEIVED, 0, NULL);
  }

  if (!read_callback_.is_null()) {
    int rv = PopulateUserReadBuffer(user_buffer_->data(), user_buffer_len_);
    CompletionCallback c = read_callback_;
    read_callback_.Reset();
    user_buffer_ = NULL;
    user_buffer_len_ = 0;
    c.Run(rv);
  }
}
