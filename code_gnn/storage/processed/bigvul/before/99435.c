  void Insert(const std::string& channel_id, int fd) {
    AutoLock locked(lock_);
    DCHECK(fd != -1);

    ChannelToFDMap::const_iterator i = map_.find(channel_id);
    CHECK(i == map_.end()) << "Creating second IPC server (fd " << fd << ") "
                           << "for '" << channel_id << "' while first "
                           << "(fd " << i->second << ") still exists";
    map_[channel_id] = fd;
  }
