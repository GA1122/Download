ExtensionResource Extension::GetResource(
    const std::string& relative_path) const {
  std::string new_path = relative_path;
  if (!new_path.empty() && new_path.at(0) == '/')
    new_path.erase(0, 1);
#if defined(OS_POSIX)
  FilePath relative_file_path(new_path);
#elif defined(OS_WIN)
  FilePath relative_file_path(UTF8ToWide(new_path));
#endif
  ExtensionResource r(id(), path(), relative_file_path);
  if ((creation_flags() & Extension::FOLLOW_SYMLINKS_ANYWHERE)) {
    r.set_follow_symlinks_anywhere();
  }
  return r;
}
