int SocketStream::DoReadWrite(int result) {
  if (result < OK) {
    next_state_ = STATE_CLOSE;
    return result;
  }
  if (!socket_.get() || !socket_->IsConnected()) {
    next_state_ = STATE_CLOSE;
    return ERR_CONNECTION_CLOSED;
  }

  if (closing_ && !current_write_buf_.get() && pending_write_bufs_.empty()) {
    socket_->Disconnect();
    next_state_ = STATE_CLOSE;
    return OK;
  }

  next_state_ = STATE_READ_WRITE;

  if (!server_closed_) {
    if (!read_buf_.get()) {
      read_buf_ = new IOBuffer(kReadBufferSize);
      result = socket_->Read(
          read_buf_.get(),
          kReadBufferSize,
          base::Bind(&SocketStream::OnReadCompleted, base::Unretained(this)));
      if (result > 0) {
        return DidReceiveData(result);
      } else if (result == 0) {
        next_state_ = STATE_CLOSE;
        server_closed_ = true;
        return ERR_CONNECTION_CLOSED;
      }
      if (result != ERR_IO_PENDING) {
        next_state_ = STATE_CLOSE;
        server_closed_ = true;
        return result;
      }
    }
    DCHECK(read_buf_.get());
  }

  if (waiting_for_write_completion_)
    return ERR_IO_PENDING;

  if (!current_write_buf_.get()) {
    if (pending_write_bufs_.empty()) {
      return ERR_IO_PENDING;
    }

    current_write_buf_ = new DrainableIOBuffer(
        pending_write_bufs_.front().get(), pending_write_bufs_.front()->size());
    pending_write_bufs_.pop_front();
  }

  result = socket_->Write(
      current_write_buf_.get(),
      current_write_buf_->BytesRemaining(),
      base::Bind(&SocketStream::OnWriteCompleted, base::Unretained(this)));

  if (result == ERR_IO_PENDING) {
    waiting_for_write_completion_ = true;
  } else if (result < 0) {
    next_state_ = STATE_CLOSE;
  } else if (result > 0) {
    DidSendData(result);
    result = OK;
  }

  return result;
}
