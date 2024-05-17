ObjectIdSet GetSequenceOfIdsStartingAt(size_t start, size_t count) {
  ObjectIdSet ids;
  for (size_t i = start; i < start + count; ++i)
    ids.insert(GetIdForIndex(i));
  return ids;
}
