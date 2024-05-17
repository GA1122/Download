void WebGLRenderingContextBase::BufferDataImpl(GLenum target,
                                               int64_t size,
                                               const void* data,
                                               GLenum usage) {
  WebGLBuffer* buffer = ValidateBufferDataTarget("bufferData", target);
  if (!buffer)
    return;

  if (!ValidateBufferDataUsage("bufferData", usage))
    return;

  if (!ValidateValueFitNonNegInt32("bufferData", "size", size))
    return;

  buffer->SetSize(size);

  ContextGL()->BufferData(target, static_cast<GLsizeiptr>(size), data, usage);
}
