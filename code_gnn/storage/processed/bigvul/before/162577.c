void Resource::SetResourceBuffer(scoped_refptr<SharedBuffer> resource_buffer) {
  DCHECK(!is_revalidating_);
  DCHECK(!ErrorOccurred());
  DCHECK_EQ(options_.data_buffering_policy, kBufferData);
  data_ = std::move(resource_buffer);
  SetEncodedSize(data_->size());
}
