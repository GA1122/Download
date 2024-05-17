  void OnDataChannelImpl(std::unique_ptr<RtcDataChannelHandler> handler) {
    DCHECK(main_thread_->BelongsToCurrentThread());
    if (handler_)
      handler_->OnDataChannel(std::move(handler));
  }
