void FileDescriptorSet::GetDescriptors(int* buffer) const {
  for (std::vector<base::FileDescriptor>::const_iterator
       i = descriptors_.begin(); i != descriptors_.end(); ++i) {
    *(buffer++) = i->fd;
   }
 }
