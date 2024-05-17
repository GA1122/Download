void CapturerMac::ScreenConfigurationChanged() {
  ReleaseBuffers();
  InvalidRects rects;
  helper_.SwapInvalidRects(rects);
  last_buffer_ = NULL;

  CGDirectDisplayID mainDevice = CGMainDisplayID();
  int width = CGDisplayPixelsWide(mainDevice);
  int height = CGDisplayPixelsHigh(mainDevice);
  InvalidateScreen(gfx::Size(width, height));

  if (CGDisplayIsBuiltin(mainDevice)) {
    VLOG(3) << "OpenGL support not available.";
    return;
  }

  CGLPixelFormatAttribute attributes[] = {
    kCGLPFAFullScreen,
    kCGLPFADisplayMask,
    (CGLPixelFormatAttribute)CGDisplayIDToOpenGLDisplayMask(mainDevice),
    (CGLPixelFormatAttribute)0
  };
  CGLPixelFormatObj pixel_format = NULL;
  GLint matching_pixel_format_count = 0;
  CGLError err = CGLChoosePixelFormat(attributes,
                                      &pixel_format,
                                      &matching_pixel_format_count);
  DCHECK_EQ(err, kCGLNoError);
  err = CGLCreateContext(pixel_format, NULL, &cgl_context_);
  DCHECK_EQ(err, kCGLNoError);
  CGLDestroyPixelFormat(pixel_format);
  CGLSetFullScreen(cgl_context_);
  CGLSetCurrentContext(cgl_context_);

  size_t buffer_size = width * height * sizeof(uint32_t);
  pixel_buffer_object_.Init(cgl_context_, buffer_size);
}
