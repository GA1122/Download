bool xmp_files_can_put_xmp_xmpstring(XmpFilePtr xf, XmpStringPtr xmp_packet)
{
  CHECK_PTR(xf, false);
  RESET_ERROR;
  SXMPFiles *txf = reinterpret_cast<SXMPFiles*>(xf);
  bool result = false;

  try {
    result = txf->CanPutXMP(*reinterpret_cast<const std::string*>(xmp_packet));
  }
  catch(const XMP_Error & e) {
    set_error(e);
    return false;
  }
  return result;
}
