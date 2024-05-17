uint32_t GLES2Util::ComputeImageGroupSize(int format, int type) {
  int bytes_per_element = BytesPerElement(type);
  DCHECK_GE(8, bytes_per_element);
  int elements_per_group = ElementsPerGroup(format, type);
  DCHECK_GE(4, elements_per_group);
  return  bytes_per_element * elements_per_group;
}
