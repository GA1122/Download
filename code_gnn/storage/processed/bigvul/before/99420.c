FileDescriptorSet::~FileDescriptorSet() {
  if (consumed_descriptor_highwater_ == descriptors_.size())
    return;

  LOG(WARNING) << "FileDescriptorSet destroyed with unconsumed descriptors";
  for (unsigned i = consumed_descriptor_highwater_;
       i < descriptors_.size(); ++i) {
    if (descriptors_[i].auto_close)
      HANDLE_EINTR(close(descriptors_[i].fd));
  }
}
