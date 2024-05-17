ModuleExport size_t RegisterSTEGANOImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("STEGANO");
  entry->decoder=(DecodeImageHandler *) ReadSTEGANOImage;
  entry->format_type=ImplicitFormatType;
  entry->description=ConstantString("Steganographic image");
  entry->module=ConstantString("STEGANO");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
