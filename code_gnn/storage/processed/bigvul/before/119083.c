  void CloseObjectFiles() {
#if !defined(NDEBUG)
    std::map<std::string, int>::iterator it;
    for (it = modules_.begin(); it != modules_.end(); ++it) {
      int ret = IGNORE_EINTR(close(it->second));
      DCHECK(!ret);
      it->second = -1;
    }
    modules_.clear();
#endif   
  }
