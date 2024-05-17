bool SharedMemory::CreateAndMapAnonymous(uint32 size) {
  return CreateAnonymous(size) && Map(size);
}
