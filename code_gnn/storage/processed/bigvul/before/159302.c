int WebGLRenderingContextBase::ExternallyAllocatedBufferCountPerPixel() {
  if (isContextLost())
    return 0;

  int buffer_count = 1;
  buffer_count *= 2;   
  int samples = GetDrawingBuffer() ? GetDrawingBuffer()->SampleCount() : 0;
  Nullable<WebGLContextAttributes> attribs;
  getContextAttributes(attribs);
  if (!attribs.IsNull()) {
    if (attribs.Get().antialias() && samples > 0 &&
        GetDrawingBuffer()->ExplicitResolveOfMultisampleData()) {
      if (attribs.Get().depth() || attribs.Get().stencil())
        buffer_count += samples;   
      buffer_count += samples;     
    } else if (attribs.Get().depth() || attribs.Get().stencil()) {
      buffer_count += 1;   
    }
  }

  return buffer_count;
}
