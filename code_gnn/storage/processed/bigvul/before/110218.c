void NaClGdbWatchDelegate::OnFileCanReadWithoutBlocking(int fd) {
  char buf;
  if (HANDLE_EINTR(read(fd_read_, &buf, 1)) != 1 || buf != '\0')
    LOG(ERROR) << "Failed to sync with nacl-gdb";
  BrowserThread::PostTask(BrowserThread::IO, FROM_HERE, reply_);
}
