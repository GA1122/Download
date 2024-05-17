bool xmp_files_put_xmp_xmpstring(XmpFilePtr xf, XmpStringPtr xmp_packet)
{
  CHECK_PTR(xf, false);
  CHECK_PTR(xmp_packet, false);
  RESET_ERROR;
  SXMPFiles *txf = reinterpret_cast<SXMPFiles*>(xf);

  try {
    txf->PutXMP(*reinterpret_cast<const std::string*>(xmp_packet));
  }
  catch(const XMP_Error & e) {
    set_error(e);
    return false;
  }
  return true;
}
