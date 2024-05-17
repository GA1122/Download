  void HandleRequestFromRenderer(int fd) {
    std::vector<int> fds;

    char buf[FontConfigInterface::kMaxFontFamilyLength + 128];

    const ssize_t len = base::RecvMsg(fd, buf, sizeof(buf), &fds);
    if (len == -1) {
      NOTREACHED()
          << "Sandbox host message is larger than kMaxFontFamilyLength";
      return;
    }
    if (fds.size() == 0)
      return;

    Pickle pickle(buf, len);
    void* iter = NULL;

    int kind;
    if (!pickle.ReadInt(&iter, &kind))
      goto error;

    if (kind == FontConfigIPC::METHOD_MATCH) {
      HandleFontMatchRequest(fd, pickle, iter, fds);
    } else if (kind == FontConfigIPC::METHOD_OPEN) {
      HandleFontOpenRequest(fd, pickle, iter, fds);
    } else if (kind == LinuxSandbox::METHOD_GET_FONT_FAMILY_FOR_CHARS) {
      HandleGetFontFamilyForChars(fd, pickle, iter, fds);
    } else if (kind == LinuxSandbox::METHOD_LOCALTIME) {
      HandleLocaltime(fd, pickle, iter, fds);
    } else if (kind == LinuxSandbox::METHOD_GET_CHILD_WITH_INODE) {
      HandleGetChildWithInode(fd, pickle, iter, fds);
    } else if (kind == LinuxSandbox::METHOD_GET_STYLE_FOR_STRIKE) {
      HandleGetStyleForStrike(fd, pickle, iter, fds);
    } else if (kind == LinuxSandbox::METHOD_MAKE_SHARED_MEMORY_SEGMENT) {
      HandleMakeSharedMemorySegment(fd, pickle, iter, fds);
    }

  error:
    for (std::vector<int>::const_iterator
         i = fds.begin(); i != fds.end(); ++i) {
      close(*i);
    }
  }
