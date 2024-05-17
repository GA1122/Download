base::FilePath BackendImpl::GetFileName(Addr address) const {
  if (!address.is_separate_file() || !address.is_initialized()) {
    NOTREACHED();
    return base::FilePath();
  }

  std::string tmp = base::StringPrintf("f_%06x", address.FileNumber());
  return path_.AppendASCII(tmp);
}
