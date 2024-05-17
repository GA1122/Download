void Resource::AppendData(const char* data, size_t length) {
  TRACE_EVENT0("blink", "Resource::appendData");
  DCHECK(!is_revalidating_);
  DCHECK(!ErrorOccurred());
  if (options_.data_buffering_policy == kBufferData) {
    if (data_)
      data_->Append(data, length);
    else
      data_ = SharedBuffer::Create(data, length);
    SetEncodedSize(data_->size());
  }
  ResourceClientWalker<ResourceClient> w(Clients());
  while (ResourceClient* c = w.Next())
    c->DataReceived(this, data, length);
}
