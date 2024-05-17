IdAllocator* GLES2Implementation::GetIdAllocator(
    IdNamespaces namespace_id) const {
  return id_allocators_[static_cast<int>(namespace_id)].get();
}
