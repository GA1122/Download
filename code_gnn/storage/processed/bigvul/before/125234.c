  void SendReplyAndDeleteThis() {
    filter_->Send(reply_msg_);
    delete this;
  }
