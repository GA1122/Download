ModuleExport size_t RegisterLABELImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("LABEL");
  entry->decoder=(DecodeImageHandler *) ReadLABELImage;
  entry->adjoin=MagickFalse;
  entry->format_type=ImplicitFormatType;
  entry->description=ConstantString("Image label");
  entry->module=ConstantString("LABEL");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
