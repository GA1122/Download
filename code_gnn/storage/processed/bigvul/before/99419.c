void FileDescriptorSet::SetDescriptors(const int* buffer, unsigned count) {
  DCHECK_LE(count, MAX_DESCRIPTORS_PER_MESSAGE);
  DCHECK_EQ(descriptors_.size(), 0u);
  DCHECK_EQ(consumed_descriptor_highwater_, 0u);

  descriptors_.reserve(count);
  for (unsigned i = 0; i < count; ++i) {
    struct base::FileDescriptor sd;
    sd.fd = buffer[i];
    sd.auto_close = true;
    descriptors_.push_back(sd);
  }
}
