 void FileDescriptorSet::CommitAll() {
   for (std::vector<base::FileDescriptor>::iterator
        i = descriptors_.begin(); i != descriptors_.end(); ++i) {
    if (i->auto_close)
      HANDLE_EINTR(close(i->fd));
  }
  descriptors_.clear();
  consumed_descriptor_highwater_ = 0;
}
