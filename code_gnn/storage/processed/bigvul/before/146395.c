bool WebGLRenderingContextBase::ValidateAndUpdateBufferBindTarget(
    const char* function_name,
    GLenum target,
    WebGLBuffer* buffer) {
  if (!ValidateBufferTarget(function_name, target))
    return false;

  if (buffer && buffer->GetInitialTarget() &&
      buffer->GetInitialTarget() != target) {
    SynthesizeGLError(GL_INVALID_OPERATION, function_name,
                      "buffers can not be used with multiple targets");
    return false;
  }

  switch (target) {
    case GL_ARRAY_BUFFER:
      bound_array_buffer_ = buffer;
      break;
    case GL_ELEMENT_ARRAY_BUFFER:
      bound_vertex_array_object_->SetElementArrayBuffer(buffer);
      break;
    default:
      NOTREACHED();
      return false;
  }

  if (buffer && !buffer->GetInitialTarget())
    buffer->SetInitialTarget(target);
  return true;
}
