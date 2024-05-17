template <typename T> static inline bool CheckIdType(T id, PPIdType type) {
  if (!id)
    return true;
  const T mask = (static_cast<T>(1) << kPPIdTypeBits) - 1;
  return (id & mask) == type;
}
