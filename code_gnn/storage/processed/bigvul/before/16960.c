int GetNextCookieRequestId() {
  static int id = 0;
  if (id == std::numeric_limits<int>::max()) {
    int i = id;
    id = 0;
    return i;
  }
  return id++;
}
