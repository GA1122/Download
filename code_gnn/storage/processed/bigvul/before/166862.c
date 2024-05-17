bool IsUninitializedMemory(void* object_pointer, size_t object_size) {
  Address* object_fields = reinterpret_cast<Address*>(object_pointer);
  for (size_t i = 0; i < object_size / sizeof(Address); ++i) {
    if (object_fields[i])
      return false;
  }
  return true;
}
