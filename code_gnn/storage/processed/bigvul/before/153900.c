bool GLES2DecoderImpl::ChromiumImageNeedsRGBEmulation() {
  gpu::ImageFactory* factory = GetContextGroup()->image_factory();
  return factory ? !factory->SupportsFormatRGB() : false;
}
