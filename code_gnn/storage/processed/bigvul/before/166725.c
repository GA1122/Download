int FreeList::BucketIndexForSize(size_t size) {
  DCHECK_GT(size, 0u);
  int index = -1;
  while (size) {
    size >>= 1;
    index++;
  }
  return index;
}
