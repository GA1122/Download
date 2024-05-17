int WebGLRenderingContextBase::ExternallyAllocatedBufferCountPerPixel() {
  if (isContextLost())
    return 0;

  int buffer_count = 1;
  buffer_count *= 2;   
  int samples = GetDrawingBuffer() ? GetDrawingBuffer()->SampleCount() : 0;
  WebGLContextAttributes* attribs = getContextAttributes();
  if (attribs) {
    if (attribs->antialias() && samples > 0 &&
        GetDrawingBuffer()->ExplicitResolveOfMultisampleData()) {
      if (attribs->depth() || attribs->stencil())
        buffer_count += samples;   
      buffer_count += samples;     
    } else if (attribs->depth() || attribs->stencil()) {
      buffer_count += 1;   
    }
  }

  return buffer_count;
}
