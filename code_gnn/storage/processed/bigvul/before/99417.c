int FileDescriptorSet::GetDescriptorAt(unsigned index) const {
  if (index >= descriptors_.size())
    return -1;

  if (index == 0 && consumed_descriptor_highwater_ == descriptors_.size())
    consumed_descriptor_highwater_ = 0;

  if (index != consumed_descriptor_highwater_)
    return -1;

  consumed_descriptor_highwater_ = index + 1;
  return descriptors_[index].fd;
}
