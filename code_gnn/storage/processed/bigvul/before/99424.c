int ChannelNameToFD(const std::string& channel_id) {
  const int fd = Singleton<PipeMap>()->Lookup(channel_id);

  if (fd != -1) {
    int dup_fd = dup(fd);
    if (dup_fd < 0)
      PLOG(FATAL) << "dup(" << fd << ")";
    return dup_fd;
  }

  return fd;
}
