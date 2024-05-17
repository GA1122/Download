void BackendImpl::DeleteBlock(Addr block_address, bool deep) {
  block_files_.DeleteBlock(block_address, deep);
}
