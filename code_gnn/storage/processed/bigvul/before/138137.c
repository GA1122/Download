static int computeBestScrollOffset(int currentScrollOffset,
                                   int subfocusMin,
                                   int subfocusMax,
                                   int objectMin,
                                   int objectMax,
                                   int viewportMin,
                                   int viewportMax) {
  int viewportSize = viewportMax - viewportMin;

  if (objectMax - objectMin > viewportSize) {
    if (subfocusMin - currentScrollOffset >= viewportMin &&
        subfocusMax - currentScrollOffset <= viewportMax)
      return currentScrollOffset;

    subfocusMin = std::max(subfocusMin, objectMin);
    subfocusMax = std::min(subfocusMax, objectMax);

    if (subfocusMax - subfocusMin > viewportSize)
      subfocusMax = subfocusMin + viewportSize;

    int centeredObjectMin = (subfocusMin + subfocusMax - viewportSize) / 2;
    int centeredObjectMax = centeredObjectMin + viewportSize;

    objectMin = std::max(objectMin, centeredObjectMin);
    objectMax = std::min(objectMax, centeredObjectMax);
  }

  if (objectMin - currentScrollOffset >= viewportMin &&
      objectMax - currentScrollOffset <= viewportMax)
    return currentScrollOffset;

  return (objectMin + objectMax - viewportMin - viewportMax) / 2;
}
