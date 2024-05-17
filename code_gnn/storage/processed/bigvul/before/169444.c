int FileStream::Open(const base::FilePath& path, int open_flags,
                     const CompletionCallback& callback) {
  if (IsOpen()) {
    DLOG(FATAL) << "File is already open!";
    return ERR_UNEXPECTED;
  }

  DCHECK(open_flags & base::File::FLAG_ASYNC);
  context_->Open(path, open_flags, callback);
  return ERR_IO_PENDING;
}
