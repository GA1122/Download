bool FileDescriptorSet::AddAndAutoClose(int fd) {
  if (descriptors_.size() == MAX_DESCRIPTORS_PER_MESSAGE)
    return false;

  struct base::FileDescriptor sd;
  sd.fd = fd;
  sd.auto_close = true;
  descriptors_.push_back(sd);
  DCHECK(descriptors_.size() <= MAX_DESCRIPTORS_PER_MESSAGE);
  return true;
}
