bool xmp_files_get_xmp_xmpstring(XmpFilePtr xf, XmpStringPtr xmp_packet,
                                 XmpPacketInfo* packet_info)
{
  CHECK_PTR(xf, false);
  CHECK_PTR(xmp_packet, false);
  RESET_ERROR;
  bool result = false;
  try {
    SXMPFiles *txf = reinterpret_cast<SXMPFiles*>(xf);

    XMP_PacketInfo xmp_packet_info;
    result = txf->GetXMP(NULL,
                         reinterpret_cast<std::string*>(xmp_packet),
                         &xmp_packet_info);
    if (packet_info) {
      packet_info->offset = xmp_packet_info.offset;
      packet_info->length = xmp_packet_info.length;
      packet_info->padSize = xmp_packet_info.padSize;
      packet_info->charForm = xmp_packet_info.charForm;
      packet_info->writeable = xmp_packet_info.writeable;
      packet_info->hasWrapper = xmp_packet_info.hasWrapper;
      packet_info->pad = xmp_packet_info.pad;
    }
  }
  catch(const XMP_Error & e) {
    set_error(e);
  }
  return result;
}
