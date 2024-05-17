  virtual std::wstring GetMessageTextWithOffset(size_t* link_offset) const {
    std::wstring message(L"Oops! Your printer does not support PDF. Please "
                         L"report this to us .");
    *link_offset = message.length() - 1;
    return message;
  }
