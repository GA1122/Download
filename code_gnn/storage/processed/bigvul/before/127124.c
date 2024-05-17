bool SharedMemory::Open(const std::string& name, bool read_only) {
  DCHECK(!mapped_file_);

  name_ = ASCIIToWide(name);
  read_only_ = read_only;
  mapped_file_ = OpenFileMapping(
      read_only_ ? FILE_MAP_READ : FILE_MAP_ALL_ACCESS, false,
      name_.empty() ? NULL : name_.c_str());
  if (mapped_file_ != NULL) {
    return true;
  }
  return false;
}
