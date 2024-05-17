MappedFile* BackendImpl::File(Addr address) {
  if (disabled_)
    return NULL;
  return block_files_.GetFile(address);
}
