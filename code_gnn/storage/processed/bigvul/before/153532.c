bool UpdateValue(T* dest, const T& src) {
  if (*dest == src)
    return false;
  *dest = src;
  return true;
}
