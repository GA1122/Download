bool GlobalHistogramAllocator::ParseFilePath(const FilePath& path,
                                             std::string* out_name,
                                             Time* out_stamp,
                                             ProcessId* out_pid) {
  std::string filename = path.BaseName().AsUTF8Unsafe();
  std::vector<base::StringPiece> parts = base::SplitStringPiece(
      filename, "-.", base::KEEP_WHITESPACE, base::SPLIT_WANT_ALL);
  if (parts.size() != 4)
    return false;

  if (out_name)
    *out_name = parts[0].as_string();

  if (out_stamp) {
    int64_t stamp;
    if (!HexStringToInt64(parts[1], &stamp))
      return false;
    *out_stamp = Time::FromTimeT(static_cast<time_t>(stamp));
  }

  if (out_pid) {
    int64_t pid;
    if (!HexStringToInt64(parts[2], &pid))
      return false;
    *out_pid = static_cast<ProcessId>(pid);
  }

  return true;
}
