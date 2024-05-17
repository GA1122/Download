bool GLES2Util::IsIntegerFormat(uint32_t internal_format) {
  return (IsUnsignedIntegerFormat(internal_format) ||
          IsSignedIntegerFormat(internal_format));
}
