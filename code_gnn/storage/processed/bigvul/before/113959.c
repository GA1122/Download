invalidation::ObjectId GetIdForIndex(size_t index) {
  char name[2] = "a";
  name[0] += static_cast<char>(index);
  return invalidation::ObjectId(1 + index, name);
}
