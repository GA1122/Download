  void Alert(const base::string16& message) {
    alerts_.push_back(base::UTF16ToASCII(message));
  }
