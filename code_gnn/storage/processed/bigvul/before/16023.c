bool xmp_files_put_xmp_cstr(XmpFilePtr xf, const char* xmp_packet, size_t len)
{
  CHECK_PTR(xf, false);
  CHECK_PTR(xmp_packet, false);
  RESET_ERROR;
  SXMPFiles *txf = reinterpret_cast<SXMPFiles*>(xf);

  try {
    txf->PutXMP(xmp_packet, len);
  }
  catch(const XMP_Error & e) {
    set_error(e);
    return false;
  }
  return true;
}
