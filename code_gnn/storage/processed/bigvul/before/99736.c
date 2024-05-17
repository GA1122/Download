void VP9Decoder::SetStream(const uint8_t* ptr, size_t size) {
  DCHECK(ptr);
  DCHECK(size);

  DVLOG(4) << "New input stream at: " << (void*)ptr << " size: " << size;
  parser_.SetStream(ptr, size);
}
