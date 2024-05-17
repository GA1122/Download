bool HFSIterator::SeekToBlock(uint64_t block) {
  uint64_t offset = block * volume_header_.blockSize;
  off_t rv = stream_->Seek(offset, SEEK_SET);
  return rv >= 0 && static_cast<uint64_t>(rv) == offset;
}
