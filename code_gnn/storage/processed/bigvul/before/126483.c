int CompareGdkRectangles(const void* p1, const void* p2) {
  int p1_x = static_cast<const GdkRectangle*>(p1)->x;
  int p2_x = static_cast<const GdkRectangle*>(p2)->x;
  if (p1_x < p2_x)
    return -1;
  else if (p1_x == p2_x)
    return 0;
  return 1;
}
