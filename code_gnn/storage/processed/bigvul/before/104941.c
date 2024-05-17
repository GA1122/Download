template <typename T> static inline T MakeTypedId(T value, PPIdType type) {
  return (value << kPPIdTypeBits) | static_cast<T>(type);
}
