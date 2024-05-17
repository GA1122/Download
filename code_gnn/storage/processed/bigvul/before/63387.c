ModuleExport size_t RegisterSCREENSHOTImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("SCREENSHOT");
  entry->decoder=(DecodeImageHandler *) ReadSCREENSHOTImage;
  entry->format_type=ImplicitFormatType;
  entry->description=ConstantString("Screen shot");
  entry->module=ConstantString("SCREENSHOT");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
