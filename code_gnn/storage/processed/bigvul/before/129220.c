khronos_uint64_t CityHashForAngle(const char* name, unsigned int len) {
  return static_cast<khronos_uint64_t>(
      CityHash64(name, static_cast<size_t>(len)));
}
