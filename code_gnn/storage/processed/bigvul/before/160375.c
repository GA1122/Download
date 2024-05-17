static bool isUninitializedMemory(void* objectPointer, size_t objectSize) {
  Address* objectFields = reinterpret_cast<Address*>(objectPointer);
  for (size_t i = 0; i < objectSize / sizeof(Address); ++i) {
    if (objectFields[i] != 0)
      return false;
  }
  return true;
}
