DrawingBuffer::GpuMemoryBufferColorBufferParameters() {
#if defined(OS_MACOSX)
  ColorBufferParameters parameters;
  parameters.target = GC3D_TEXTURE_RECTANGLE_ARB;

  if (want_alpha_channel_) {
    parameters.allocate_alpha_channel = true;
  } else if (ContextProvider()
                 ->GetCapabilities()
                 .chromium_image_rgb_emulation) {
    parameters.allocate_alpha_channel = false;
  } else {
    parameters.allocate_alpha_channel =
        DefaultBufferRequiresAlphaChannelToBePreserved();
  }
  return parameters;
#else
  return TextureColorBufferParameters();
#endif
}
