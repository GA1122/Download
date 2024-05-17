bool CreateTemporaryFileInDir(const FilePath& dir, FilePath* temp_file) {
  base::ThreadRestrictions::AssertIOAllowed();   
  int fd = CreateAndOpenFdForTemporaryFile(dir, temp_file);
  return ((fd >= 0) && !HANDLE_EINTR(close(fd)));
}
