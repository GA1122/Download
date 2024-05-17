  void CheckAccepted() {
    DCHECK(num_accepted_connections_loop_ ||
           num_accepted_connections_needed_ == 0);
    if (!num_accepted_connections_loop_ ||
        num_accepted_connections_needed_ != sockets_.size()) {
      return;
    }

    num_accepted_connections_loop_->Quit();
    num_accepted_connections_needed_ = 0;
    num_accepted_connections_loop_ = nullptr;
  }
