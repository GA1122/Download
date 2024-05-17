unsigned long GetFontData(struct _FPDF_SYSFONTINFO*, void* font_id,
                          unsigned int table, unsigned char* buffer,
                          unsigned long buf_size) {
  if (!pp::PDF::IsAvailable()) {
    NOTREACHED();
    return 0;
  }

  uint32_t size = buf_size;
  long res_id = reinterpret_cast<long>(font_id);
  if (!pp::PDF::GetFontTableForPrivateFontFile(res_id, table, buffer, &size))
    return 0;
  return size;
}
