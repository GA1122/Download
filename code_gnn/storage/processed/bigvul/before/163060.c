void SandboxIPCHandler::HandleMatchWithFallback(
    int fd,
    base::PickleIterator iter,
    const std::vector<base::ScopedFD>& fds) {
  std::string face;
  bool is_bold;
  bool is_italic;
  uint32_t charset;
  uint32_t fallback_family;

  if (!iter.ReadString(&face) || face.empty() || !iter.ReadBool(&is_bold) ||
      !iter.ReadBool(&is_italic) || !iter.ReadUInt32(&charset) ||
      !iter.ReadUInt32(&fallback_family)) {
    return;
  }

  int font_fd = MatchFontFaceWithFallback(face, is_bold, is_italic, charset,
                                          fallback_family);

  base::Pickle reply;
  SendRendererReply(fds, reply, font_fd);

  if (font_fd >= 0) {
    if (IGNORE_EINTR(close(font_fd)) < 0)
      PLOG(ERROR) << "close";
  }
}
