  void HandleFontMatchRequest(int fd, const Pickle& pickle, void* iter,
                              std::vector<int>& fds) {
    bool fileid_valid;
    uint32_t fileid;

    if (!pickle.ReadBool(&iter, &fileid_valid))
      return;
    if (fileid_valid) {
      if (!pickle.ReadUInt32(&iter, &fileid))
        return;
    }
    bool is_bold, is_italic;
    if (!pickle.ReadBool(&iter, &is_bold) ||
        !pickle.ReadBool(&iter, &is_italic)) {
      return;
    }

    uint32_t characters_bytes;
    if (!pickle.ReadUInt32(&iter, &characters_bytes))
      return;
    const char* characters = NULL;
    if (characters_bytes > 0) {
      const uint32_t kMaxCharactersBytes = 1 << 10;
      if (characters_bytes % 2 == 0 ||   
          characters_bytes > kMaxCharactersBytes ||
          !pickle.ReadBytes(&iter, &characters, characters_bytes))
        return;
    }

    std::string family;
    if (!pickle.ReadString(&iter, &family))
      return;

    std::string result_family;
    unsigned result_fileid;
    const bool r = font_config_->Match(
        &result_family, &result_fileid, fileid_valid, fileid, family,
        characters, characters_bytes, &is_bold, &is_italic);

    Pickle reply;
    if (!r) {
      reply.WriteBool(false);
    } else {
      reply.WriteBool(true);
      reply.WriteUInt32(result_fileid);
      reply.WriteString(result_family);
      reply.WriteBool(is_bold);
      reply.WriteBool(is_italic);
    }
    SendRendererReply(fds, reply, -1);
  }
