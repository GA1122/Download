 bool SetExtendedFileAttribute(const char* path,
                               const char* name,
                                const char* value,
                                size_t value_size,
                                int flags) {
  
  
// #if !defined(OS_CHROMEOS)
    base::ScopedBlockingCall scoped_blocking_call(base::BlockingType::MAY_BLOCK);
    int result = setxattr(path, name, value, value_size, flags);
    if (result) {
      DPLOG(ERROR) << "Could not set extended attribute " << name << " on file "
                   << path;
      return false;
    }
// #endif   
    return true;
  }