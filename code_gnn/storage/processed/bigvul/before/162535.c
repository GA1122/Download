void Resource::DidAddClient(ResourceClient* c) {
  if (scoped_refptr<SharedBuffer> data = Data()) {
    data->ForEachSegment([this, &c](const char* segment, size_t segment_size,
                                    size_t segment_offset) -> bool {
      c->DataReceived(this, segment, segment_size);

      return HasClient(c);
    });
  }
  if (!HasClient(c))
    return;
  if (IsLoaded()) {
    c->NotifyFinished(this);
    if (clients_.Contains(c)) {
      finished_clients_.insert(c);
      clients_.erase(c);
    }
  }
}
