void NaClDescCustomDestroy(void* handle) {
  delete static_cast<DescThunker*>(handle);
}
