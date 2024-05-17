static bool getInternalOption(
 const void *data, size_t size, T *out) {
 if (size != sizeof(T)) {
 return false;
 }
 *out = *(T*)data;
 return true;
}
