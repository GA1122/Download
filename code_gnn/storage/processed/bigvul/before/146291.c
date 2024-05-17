int WebGLRenderingContextBase::ExternallyAllocatedBytesPerPixel() {
  if (isContextLost())
    return 0;

  int bytes_per_pixel = 4;
  int total_bytes_per_pixel =
      bytes_per_pixel * 2;   
  int samples = GetDrawingBuffer() ? GetDrawingBuffer()->SampleCount() : 0;
  Nullable<WebGLContextAttributes> attribs;
  getContextAttributes(attribs);
  if (!attribs.IsNull()) {
    if (attribs.Get().antialias() && samples > 0 &&
        GetDrawingBuffer()->ExplicitResolveOfMultisampleData()) {
      if (attribs.Get().depth() || attribs.Get().stencil())
        total_bytes_per_pixel +=
            samples * bytes_per_pixel;   
      total_bytes_per_pixel +=
          samples * bytes_per_pixel;   
    } else if (attribs.Get().depth() || attribs.Get().stencil()) {
      total_bytes_per_pixel += bytes_per_pixel;   
    }
  }

  return total_bytes_per_pixel;
}
