 bool DrawingBuffer::ShouldUseChromiumImage() {
  return RuntimeEnabledFeatures::WebGLImageChromiumEnabled() &&
         chromium_image_usage_ == kAllowChromiumImage;
}
