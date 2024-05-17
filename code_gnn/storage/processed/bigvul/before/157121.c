scoped_refptr<DataBuffer> ResourceMultiBufferDataProvider::Read() {
  DCHECK(Available());
  scoped_refptr<DataBuffer> ret = fifo_.front();
  fifo_.pop_front();
  ++pos_;
  return ret;
}
