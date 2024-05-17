bool HasEnoughSpaceFor(int64 num_bytes) {
  int64 free_space = GetAmountOfFreeDiskSpace();
  free_space -= kMinFreeSpace;
  return (free_space >= num_bytes);
}
