bool RealPath(const FilePath& path, FilePath* real_path) {
  base::ThreadRestrictions::AssertIOAllowed();   
  FilePath::CharType buf[PATH_MAX];
  if (!realpath(path.value().c_str(), buf))
    return false;

  *real_path = FilePath(buf);
  return true;
}
