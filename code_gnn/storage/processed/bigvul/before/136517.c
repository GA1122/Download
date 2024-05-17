static FloatRect ComputeBottomDelta(const FloatPoint& location,
                                    const FloatSize& old_size,
                                    const FloatSize& new_size) {
  float delta = new_size.Height() - old_size.Height();
  if (delta > 0) {
    return FloatRect(location.X(), location.Y() + old_size.Height(),
                     new_size.Width(), delta);
  }
  if (delta < 0) {
    return FloatRect(location.X(), location.Y() + new_size.Height(),
                     old_size.Width(), -delta);
  }
  return FloatRect();
}
