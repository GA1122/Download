  void HandleGetFontFamilyForChars(int fd, const Pickle& pickle, void* iter,
                                   std::vector<int>& fds) {

    int num_chars;
    if (!pickle.ReadInt(&iter, &num_chars))
      return;

    static const int kMaxChars = 4096;
    if (num_chars < 1 || num_chars > kMaxChars) {
      LOG(WARNING) << "HandleGetFontFamilyForChars: too many chars: "
                   << num_chars;
      return;
    }

    scoped_array<WebUChar> chars(new WebUChar[num_chars]);

    for (int i = 0; i < num_chars; ++i) {
      uint32_t c;
      if (!pickle.ReadUInt32(&iter, &c)) {
        return;
      }

      chars[i] = c;
    }

    WebCString family = WebFontInfo::familyForChars(chars.get(), num_chars);

    Pickle reply;
    if (family.data()) {
      reply.WriteString(family.data());
    } else {
      reply.WriteString("");
    }
    SendRendererReply(fds, reply, -1);
  }
