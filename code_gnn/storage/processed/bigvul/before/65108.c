ModuleExport size_t RegisterDDSImage(void)
{
  MagickInfo
    *entry;

  entry = SetMagickInfo("DDS");
  entry->decoder = (DecodeImageHandler *) ReadDDSImage;
  entry->encoder = (EncodeImageHandler *) WriteDDSImage;
  entry->magick = (IsImageFormatHandler *) IsDDS;
  entry->seekable_stream=MagickTrue;
  entry->description = ConstantString("Microsoft DirectDraw Surface");
  entry->module = ConstantString("DDS");
  (void) RegisterMagickInfo(entry);
  entry = SetMagickInfo("DXT1");
  entry->decoder = (DecodeImageHandler *) ReadDDSImage;
  entry->encoder = (EncodeImageHandler *) WriteDDSImage;
  entry->magick = (IsImageFormatHandler *) IsDDS;
  entry->seekable_stream=MagickTrue;
  entry->description = ConstantString("Microsoft DirectDraw Surface");
  entry->module = ConstantString("DDS");
  (void) RegisterMagickInfo(entry);
  entry = SetMagickInfo("DXT5");
  entry->decoder = (DecodeImageHandler *) ReadDDSImage;
  entry->encoder = (EncodeImageHandler *) WriteDDSImage;
  entry->magick = (IsImageFormatHandler *) IsDDS;
  entry->seekable_stream=MagickTrue;
  entry->description = ConstantString("Microsoft DirectDraw Surface");
  entry->module = ConstantString("DDS");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
