void PrintingMessageFilter::OnAllocateTempFileForPrinting(
    base::FileDescriptor* temp_file_fd, int* sequence_number) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));
  temp_file_fd->fd = *sequence_number = -1;
  temp_file_fd->auto_close = false;

  SequenceToPathMap* map = &g_printing_file_descriptor_map.Get().map;
  *sequence_number = g_printing_file_descriptor_map.Get().sequence++;

  FilePath path;
  if (file_util::CreateTemporaryFile(&path)) {
    int fd = open(path.value().c_str(), O_WRONLY);
    if (fd >= 0) {
      SequenceToPathMap::iterator it = map->find(*sequence_number);
      if (it != map->end()) {
        NOTREACHED() << "Sequence number already in use. seq=" <<
            *sequence_number;
      } else {
        (*map)[*sequence_number] = path;
        temp_file_fd->fd = fd;
        temp_file_fd->auto_close = true;
      }
    }
  }
}
