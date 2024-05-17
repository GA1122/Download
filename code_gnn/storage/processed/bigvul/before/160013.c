bool BackendImpl::CreateBlock(FileType block_type, int block_count,
                             Addr* block_address) {
  return block_files_.CreateBlock(block_type, block_count, block_address);
}
