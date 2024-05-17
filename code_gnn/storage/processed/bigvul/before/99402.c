  void HandleFontOpenRequest(int fd, const Pickle& pickle, void* iter,
                             std::vector<int>& fds) {
    uint32_t fileid;
    if (!pickle.ReadUInt32(&iter, &fileid))
      return;
    const int result_fd = font_config_->Open(fileid);

    Pickle reply;
    if (result_fd == -1) {
      reply.WriteBool(false);
    } else {
      reply.WriteBool(true);
    }

    SendRendererReply(fds, reply, result_fd);

    if (result_fd >= 0)
      close(result_fd);
  }
