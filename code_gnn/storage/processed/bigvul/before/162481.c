scoped_refptr<const SharedBuffer> ImageResource::ResourceBuffer() const {
  if (Data())
    return Data();
  return GetContent()->ResourceBuffer();
}
