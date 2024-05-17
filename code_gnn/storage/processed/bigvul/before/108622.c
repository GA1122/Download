bool FingerprintsEqual(const HashValueVector& a, const HashValueVector& b) {
  size_t size = a.size();

  if (size != b.size())
    return false;

  for (size_t i = 0; i < size; ++i) {
    if (!a[i].Equals(b[i]))
      return false;
  }

  return true;
}
