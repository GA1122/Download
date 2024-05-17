static STGMEDIUM* GetStorageForString(const std::basic_string<T>& data) {
  return GetStorageForBytes(
      data.c_str(),
      (data.size() + 1) * sizeof(std::basic_string<T>::value_type));
}
