  int GetFileDescriptor(const char* file_path) {
    int fd = -1;

#if !defined(NDEBUG)
    if (file_path) {
      std::map<std::string, int>::const_iterator it;
      for (it = modules_.begin(); it != modules_.end(); ++it) {
        if (strcmp((it->first).c_str(), file_path) == 0) {
          fd = dup(it->second);
          break;
        }
      }
      if (fd >= 0 && lseek(fd, 0, SEEK_SET) < 0) {
        fd = -1;
      }
    }
#endif   

    return fd;
  }
