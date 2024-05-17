bool FileDescriptorSet::Add(int fd) {
  if (descriptors_.size() == MAX_DESCRIPTORS_PER_MESSAGE)
    return false;

  struct base::FileDescriptor sd;
  sd.fd = fd;
  sd.auto_close = false;
  descriptors_.push_back(sd);
  return true;
}
