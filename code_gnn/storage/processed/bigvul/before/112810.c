void ChromeMockRenderThread::OnAllocateTempFileForPrinting(
    base::FileDescriptor* renderer_fd,
    int* browser_fd) {
  renderer_fd->fd = *browser_fd = -1;
  renderer_fd->auto_close = false;

  FilePath path;
  if (file_util::CreateTemporaryFile(&path)) {
    int fd = open(path.value().c_str(), O_WRONLY);
    DCHECK_GE(fd, 0);
    renderer_fd->fd = *browser_fd = fd;
  }
}
