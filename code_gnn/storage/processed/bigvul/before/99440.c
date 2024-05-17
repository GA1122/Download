  void RemoveAndClose(const std::string& channel_id) {
    AutoLock locked(lock_);

    ChannelToFDMap::iterator i = map_.find(channel_id);
    if (i != map_.end()) {
      HANDLE_EINTR(close(i->second));
      map_.erase(i);
    }
  }
